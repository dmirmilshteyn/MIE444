using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Tweak.Pathfinding;
using Windows.Storage;

namespace Tweak
{
    class CodeGenerator
    {
        public static readonly int TILES_PER_LINE = 15;

        public async Task GenerateConstantsHeader(StorageFolder storageFolder, Constants constants, Map map) {
            using (Stream fileStream = await storageFolder.OpenStreamForWriteAsync("MappingConstants.h", CreationCollisionOption.ReplaceExisting)) {
                using (StreamWriter writer = new StreamWriter(fileStream)) {
                    writer.WriteLine("/**********************************************");
                    writer.WriteLine(" ** This is a generated file. Do not modify. **");
                    writer.WriteLine(" **********************************************/");
                    writer.WriteLine();

                    writer.WriteLine("#ifndef MAPPING_CONSTANTS_H");
                    writer.WriteLine("#define MAPPING_CONSTANTS_H");
                    writer.WriteLine();
                    writer.WriteLine("/**********************************************");
                    writer.WriteLine(" *********** Environment Constants ************");
                    writer.WriteLine(" **********************************************/");
                    writer.WriteLine($"#define LINE_WIDTH {constants.LineWidth}");
                    writer.WriteLine($"#define MAP_WIDTH {constants.MapWidth}");
                    writer.WriteLine($"#define MAP_HEIGHT {constants.MapHeight}");
                    writer.WriteLine($"#define MAP_RESOLUTION {constants.MapResolution}");
                    writer.WriteLine();

                    double map_tiles_width = Math.Ceiling(constants.MapWidth / constants.MapResolution);
                    double map_tiles_height = Math.Ceiling(constants.MapHeight / constants.MapResolution);

                    writer.WriteLine($"#define MAP_TILES_WIDTH {map_tiles_width}");
                    writer.WriteLine($"#define MAP_TILES_HEIGHT {map_tiles_height}");
                    writer.WriteLine();

                    writer.WriteLine($"#define INTERSECTION_MARKER_COUNT {map.IntersectionMarkers.Count}");
                    writer.WriteLine($"#define STARTING_POSITION_COUNT {map.StartPositions.Count}");
                    writer.WriteLine();

                    writer.WriteLine("#endif");
                }
            }
        }

        public async Task GenerateMapHeader(StorageFolder storageFolder, Constants constants, Map map) {
            using (Stream fileStream = await storageFolder.OpenStreamForWriteAsync("Map.h", CreationCollisionOption.ReplaceExisting)) {
                using (StreamWriter writer = new StreamWriter(fileStream)) {
                    writer.WriteLine("/**********************************************");
                    writer.WriteLine(" ** This is a generated file. Do not modify. **");
                    writer.WriteLine(" **********************************************/");
                    writer.WriteLine();

                    writer.WriteLine("#ifndef MAP_H");
                    writer.WriteLine("#define MAP_H");
                    writer.WriteLine();

                    writer.WriteLine("#include \"Arduino.h\"");
                    writer.WriteLine("#include \"MappingConstants.h\"");

                    writer.WriteLine("// Map tiles are stored as a byte array, but should be manipulated as a BIT array.");
                    writer.WriteLine("// This is done to minimize memory usage as the Arduino Uno only has 2048 bytes available.");
                    writer.WriteLine();

                    int arraySize = (int)Math.Ceiling((constants.MapWidth / constants.MapResolution) * (constants.MapHeight / constants.MapResolution) / 8);

                    writer.WriteLine("// Array size = ((MAP_WIDTH / MAP_RESOLUTION) * (MAP_HEIGHT / MAP_RESOLUTION) / 8)");
                    writer.Write($"const PROGMEM byte map_tiles[{arraySize}] = {{");

                    byte[] exportedValues = map.Export();
                    for (int i = 0; i < exportedValues.Length; i++) {
                        if (i % TILES_PER_LINE == 0) {
                            writer.WriteLine();
                        }

                        writer.Write(exportedValues[i]);

                        if (i < exportedValues.Length - 1) {
                            writer.Write(", ");
                        }
                    }

                    if (exportedValues.Length - 1 % TILES_PER_LINE != 0) {
                        writer.WriteLine();
                    }

                    writer.WriteLine($"}};");
                    writer.WriteLine();

                    // Write out the intersection type constants
                    string[] names = Enum.GetNames(typeof(IntersectionType));
                    for (int i = 0; i < names.Length; i++) {
                        var intersectionTypeName = names[i];
                        writer.WriteLine($"#define INTERSECTION_TYPE_{intersectionTypeName.ToUpper()} {i}");
                    }
                    writer.WriteLine();

                    // Write out the intersection datatype
                    writer.WriteLine("struct intersection_marker {");
                    writer.WriteLine("  byte id;");
                    writer.WriteLine();
                    writer.WriteLine("  byte x1;");
                    writer.WriteLine("  byte y1;");
                    writer.WriteLine("  byte x2;");
                    writer.WriteLine("  byte y2;");
                    writer.WriteLine();
                    writer.WriteLine("  byte type;");
                    writer.WriteLine();
                    writer.WriteLine("  byte intersectionX;");
                    writer.WriteLine("  byte intersectionY;");
                    writer.WriteLine("};");
                    writer.WriteLine();

                    // Write out the values for each intersection
                    writer.WriteLine($"extern const intersection_marker intersections[INTERSECTION_MARKER_COUNT];");
                    writer.WriteLine();

                    // Write out the start position datatype
                    writer.WriteLine("struct start_position {");
                    writer.WriteLine("  byte x;");
                    writer.WriteLine("  byte y;");
                    writer.WriteLine("  byte nearestIntersectionId;");
                    writer.WriteLine("};");
                    writer.WriteLine();

                    // Write out the extern for the start positions array
                    writer.WriteLine("extern const start_position start_positions[STARTING_POSITION_COUNT];");
                    writer.WriteLine();

                    // Build the intersection costmap
                    IntersectionCostmapGenerator costmapGenerator = new IntersectionCostmapGenerator(map);
                    IntersectionGraphNode[,] costmap = costmapGenerator.BuildCostmap();

                    // Write out the costmap for intersections
                    writer.WriteLine($"extern const int8_t intersection_graph[INTERSECTION_MARKER_COUNT][INTERSECTION_MARKER_COUNT][2];");

                    writer.WriteLine();
                    writer.WriteLine("#endif");
                }
            }

            using (Stream fileStream = await storageFolder.OpenStreamForWriteAsync("Map.cpp", CreationCollisionOption.ReplaceExisting)) {
                using (StreamWriter writer = new StreamWriter(fileStream)) {
                    writer.WriteLine("/**********************************************");
                    writer.WriteLine(" ** This is a generated file. Do not modify. **");
                    writer.WriteLine(" **********************************************/");
                    writer.WriteLine();

                    writer.WriteLine("#include \"Map.h\"");
                    writer.WriteLine();

                    // Write out the values for each intersection
                    writer.WriteLine($"const PROGMEM intersection_marker intersections[INTERSECTION_MARKER_COUNT] = {{");
                    for (int i = 0; i < map.IntersectionMarkers.Count; i++) {
                        writer.Write("  { ");

                        var marker = map.IntersectionMarkers[i];

                        var intersectionLocation = DetermineIntersectionLocation(map.IntersectionMarkers, marker.IntersectionId);

                        writer.Write($"{marker.IntersectionId}, {marker.X1}, {marker.Y1}, {marker.X2}, {marker.Y2}, {(int)marker.IntersectionType}, {intersectionLocation.X}, {intersectionLocation.Y}");

                        writer.Write(" }");

                        if (i < map.IntersectionMarkers.Count - 1) {
                            writer.Write(", ");
                        }
                        writer.WriteLine();
                    }
                    writer.WriteLine("};");
                    writer.WriteLine();

                    // Write out the values for each start position
                    writer.WriteLine("const PROGMEM start_position start_positions[STARTING_POSITION_COUNT] = {");
                    for (int i = 0; i < map.StartPositions.Count; i++) {
                        writer.Write($" {{ {map.StartPositions[i].X}, {map.StartPositions[i].Y}, {map.StartPositions[i].NearestIntersectionId} }}");

                        if (i < map.StartPositions.Count - 1) {
                            writer.Write(",");
                        }
                        writer.WriteLine();
                    }
                    writer.WriteLine("};");
                    writer.WriteLine();

                    // Build the intersection costmap
                    IntersectionCostmapGenerator costmapGenerator = new IntersectionCostmapGenerator(map);
                    IntersectionGraphNode[,] costmap = costmapGenerator.BuildCostmap();

                    // Write out the costmap for intersections
                    writer.WriteLine($"const PROGMEM int8_t intersection_graph[INTERSECTION_MARKER_COUNT][INTERSECTION_MARKER_COUNT][2] = {{");
                    for (int y = 0; y < costmap.GetLength(1); y++) {
                        writer.Write(" {");
                        for (int x = 0; x < costmap.GetLength(0); x++) {
                            var graphNode = costmap[x, y];
                            if (graphNode != null) {
                                writer.Write($"  {{ {graphNode.Cost}, {(int)graphNode.IntersectionType} }}");
                            } else {
                                writer.Write("  { -1, 0 }");
                            }
                            if (x != costmap.GetLength(0) - 1) {
                                writer.Write(",");
                            }
                        }
                        writer.Write(" }");
                        if (y < costmap.GetLength(1) - 1) {
                            writer.Write(",");
                        }
                        writer.WriteLine();
                    }
                    writer.WriteLine("};");
                }
            }
        }

        private Position DetermineIntersectionLocation(IReadOnlyCollection<IntersectionMarker> markers, int targetIntersection) {
            int xTotal = 0;
            int yTotal = 0;

            int xCount = 0;
            int yCount = 0;
            foreach (var marker in markers) {
                if (marker.IntersectionId == targetIntersection) {
                    xTotal += marker.X1 + marker.X2;
                    yTotal += marker.Y1 + marker.Y2;

                    xCount += 2;
                    yCount += 2;
                }
            }

            return new Position(xTotal / xCount, yTotal / yCount);
        }
    }
}
