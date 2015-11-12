using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tweak.Pathfinding
{
    class Node
    {
        public double G { get; set; }
        public double H { get; set; }

        public double F {
            get { return G + H; }
        }

        public Node Parent { get; set; }

        public int X { get; private set; }
        public int Y { get; private set; }

        public bool Closed { get; set; }

        public Node(int x, int y) {
            this.G = 0;
            this.H = 0;

            this.X = x;
            this.Y = y;
        }
    }
}
