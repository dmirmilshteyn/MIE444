using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tweak.Bluetooth
{
    class PathPlanNode
    {
        public int MarkerId { get; set; }
        public int IntersectionId { get; set; }
        public TurnDirection TurnDirection { get; set; }
    }
}
