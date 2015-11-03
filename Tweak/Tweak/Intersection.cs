using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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
    }
}
