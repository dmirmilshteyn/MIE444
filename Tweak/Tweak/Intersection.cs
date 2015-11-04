using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation;

namespace Tweak
{
    public class Intersection
    {
        public double X { get; set; }
        public double Y { get; set; }
        public double Width { get; set; }
        public double Height { get; set; }

        public Intersection(double x, double y, double width, double height) {
            this.X = x;
            this.Y = y;
            this.Width = width;
            this.Height = height;
        }

        public Intersection() {
        }

        public Tuple<int, int, int, int> Export(Map map) {
            int topLeft = (int)(Y * map.Tiles.Width + X);
            int topRight = (int)(topLeft + Width);
            int bottomLeft = (int)(topLeft + Height * map.Tiles.Width);
            int bottomRight = (int)(topRight + Height * map.Tiles.Width);

            return Tuple.Create(topLeft, topRight, bottomLeft, bottomRight);
        }
    }
}
