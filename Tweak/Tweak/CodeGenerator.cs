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

        public async Task GenerateConstantsHeader(StorageFolder storageFolder, Constants constants) {
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

                    // Write out the intersection datatype
                    writer.WriteLine("struct intersection {");
                    writer.WriteLine("  short TopLeft;");
                    writer.WriteLine("  short TopRight;");
                    writer.WriteLine("  short BottomLeft;");
                    writer.WriteLine("  short BottomRight;");
                    writer.WriteLine("};");
                    writer.WriteLine();

                    // Write out the values for each intersection
                    writer.WriteLine($"intersection intersections[{map.Intersections.Count}] = {{");
                    for (int i = 0; i < map.Intersections.Count; i++) {
                        writer.Write("  { ");

                        var intersectionLocations = map.Intersections[i].Export(map);
                        writer.Write($"{intersectionLocations.Item1}, {intersectionLocations.Item2}, {intersectionLocations.Item3}, {intersectionLocations.Item4}");

                        writer.Write(" }");

                        if (i < map.Intersections.Count - 1) {
                            writer.Write(", ");
                        }
                        writer.WriteLine();
                    }
                    writer.WriteLine("};");
                    writer.WriteLine();

                    // Build the intersection costmap
                    IntersectionCostmapGenerator costmapGenerator = new IntersectionCostmapGenerator(map);
                    int[,] costmap = costmapGenerator.BuildCostmap();

                    // Write out the costmap for intersections
                    writer.WriteLine($"const PROGMEM byte intersection_cost_map[{costmap.GetLength(0)}][{costmap.GetLength(1)}] = {{");
                    for (int y = 0; y < costmap.GetLength(1); y++) {
                        writer.Write(" {");
                        for (int x = 0; x < costmap.GetLength(0); x++) {
                            writer.Write($" {costmap[x, y]}");
                            if (x != costmap.GetLength(0) - 1) {
                                writer.Write(",");
                            }
                        }
                        writer.Write(" }");
                        if (y != costmap.GetLength(1)) {
                            writer.Write(",");
                        }
                        writer.WriteLine();
                    }
                    writer.WriteLine("}");

                    writer.WriteLine();
                    writer.WriteLine("#endif");
                }
            }
        }
    }
}
