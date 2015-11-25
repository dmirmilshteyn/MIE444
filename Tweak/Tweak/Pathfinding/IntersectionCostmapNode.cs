using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tweak.Pathfinding
{
    class IntersectionCostmapNode
    {
        public int X { get; set; }
        public int Y { get; set; }

        public int Cost { get; set; }
        public bool Closed { get; set; }

        public int Distance { get; set; }

        public IntersectionCostmapNode Parent { get; set; }

        public IntersectionCostmapNode(int x, int y) {
            this.X = x;
            this.Y = y;

            this.Distance = -1;
            this.Parent = null;
        }
    }
}
