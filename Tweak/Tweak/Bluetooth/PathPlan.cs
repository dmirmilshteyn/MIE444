using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tweak.Bluetooth
{
    class PathPlan
    {
        public ObservableCollection<PathPlanNode> Plan { get; private set; }

        public PathPlan() {
            Plan = new ObservableCollection<PathPlanNode>();
        }
    }
}
