using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tweak
{
    public class Project : ObservableObject
    {
        Constants constants;
        public Constants Constants {
            get { return constants; }
            set {
                constants = value;
                RaisePropertyChanged();
            }
        }

        Map map;
        public Map Map {
            get { return map; }
            set {
                map = value;
                RaisePropertyChanged();
            }
        }

        public Project() {
            Constants = new Constants();
            Map = new Map();
        }

        public void Initialize() {
            Map.InitializeMap(Constants);
        }
    }
}
