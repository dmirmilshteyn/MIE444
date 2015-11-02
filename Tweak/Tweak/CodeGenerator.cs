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

                writer.WriteLine("#ifndef CONSTANTS");
                writer.WriteLine("#define CONSTANTS");
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
    }
}
