using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
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
                    writer.WriteLine(" ************** Robot Constants ***************");
                    writer.WriteLine(" **********************************************/");
                    writer.WriteLine($"const float WHEEL_RADIUS = {constants.WheelRadius};");
                    writer.WriteLine();
                    writer.WriteLine("/**********************************************");
                    writer.WriteLine(" *********** Environment Constants ************");
                    writer.WriteLine(" **********************************************/");
                    writer.WriteLine($"const float LINE_WIDTH = {constants.LineWidth};");
                    writer.WriteLine($"const int MAP_WIDTH = {constants.MapWidth};");
                    writer.WriteLine($"const int MAP_HEIGHT = {constants.MapHeight};");
                    writer.WriteLine($"const float MAP_RESOLUTION = {constants.MapResolution};");
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

                    double map_tiles_width = Math.Ceiling(constants.MapWidth / constants.MapResolution / 8);
                    double map_tiles_height = Math.Ceiling(constants.MapHeight / constants.MapResolution / 8);
                    int arraySize = (int)(map_tiles_width * map_tiles_height);

                    writer.WriteLine($"const int map_tiles_width = {map_tiles_width};");
                    writer.WriteLine($"const int map_tiles_height = {map_tiles_height};");
                    writer.WriteLine();
                    writer.WriteLine("// Array size = (MAP_WIDTH / MAP_RESOLUTION / 8) * (MAP_HEIGHT / MAP_RESOLUTION / 8)");
                    writer.Write($"byte map_tiles[{arraySize}] = {{");

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
                    writer.WriteLine("#endif");
                }
            }
        }
    }
}
