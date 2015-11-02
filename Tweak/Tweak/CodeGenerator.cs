using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tweak
{
    class CodeGenerator
    {
        public void GenerateConstantsHeader(string filePath, Constants constants) {
            using (StreamWriter writer = new StreamWriter(filePath)) {
                writer.WriteLine("/**********************************************");
                writer.WriteLine(" ** This is a generated file. Do not modify. **");
                writer.WriteLine(" **********************************************/");
                writer.WriteLine();

                writer.WriteLine("#ifndef CONSTANTS_H");
                writer.WriteLine("#define CONSTANTS_H");
                writer.WriteLine();
                writer.WriteLine("/**********************************************");
                writer.WriteLine(" ************* Hardware Constants *************");
                writer.WriteLine(" **********************************************/");
                writer.WriteLine($"const int AIN1 = {constants.AIN1};");
                writer.WriteLine($"const int AIN2 = {constants.AIN2};");
                writer.WriteLine($"const int BIN1 = {constants.BIN1};");
                writer.WriteLine($"const int BIN2 = {constants.BIN2};");
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
                writer.WriteLine($"const int MAP_RESOLUTION = {constants.MapResolution};");
                writer.WriteLine();
                writer.WriteLine("#endif");
            }
        }

        public void GenerateMapHeader(string filePath, Constants constants) {
            using (StreamWriter writer = new StreamWriter(filePath)) {
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

                float map_tiles_width = (constants.MapWidth / constants.MapResolution / 8);
                float map_tiles_height = (constants.MapHeight / constants.MapResolution / 8);
                int arraySize = (int)(map_tiles_width * map_tiles_height);

                writer.WriteLine($"const int map_tiles_width = {map_tiles_width};");
                writer.WriteLine($"const int map_tiles_height = {map_tiles_height};");
                writer.WriteLine();
                writer.WriteLine("// Array size = (MAP_WIDTH / MAP_RESOLUTION / 8) * (MAP_HEIGHT / MAP_RESOLUTION / 8)");
                writer.WriteLine($"byte map_tiles[{arraySize}];");

                writer.WriteLine();
                writer.WriteLine("#endif");
            }
        }
    }
}
