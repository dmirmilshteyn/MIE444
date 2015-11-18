using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tweak
{
    public class IntersectionMarker
    {
        public int X1 { get; set; }
        public int Y1 { get; set; }
        public int X2 { get; set; }
        public int Y2 { get; set; }

        public int IntersectionId { get; set; }

        public IntersectionType LeftType { get; set; }
        public IntersectionType RightType { get; set; }
        public IntersectionType TopType { get; set; }
        public IntersectionType BottomType { get; set; }

        public IntersectionMarker() {
        }

        public IntersectionMarker(int x1, int y1, int x2, int y2, int intersectionId, IntersectionType leftType, IntersectionType rightType) {
            this.X1 = x1;
            this.Y1 = y1;
            this.X2 = x2;
            this.Y2 = y2;

            this.IntersectionId = intersectionId;
            this.LeftType = leftType;
            this.RightType = rightType;
        }
    }
}
