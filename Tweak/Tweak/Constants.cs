using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tweak
{
    public class Constants : ObservableObject
    {
        double lineWidth;
        public double LineWidth {
            get { return lineWidth; }
            set {
                lineWidth = value;
                RaisePropertyChanged();
            }
        }

        double mapWidth;
        public double MapWidth {
            get { return mapWidth; }
            set {
                mapWidth = value;
                RaisePropertyChanged();
            }
        }

        double mapHeight;
        public double MapHeight {
            get { return mapHeight; }
            set {
                mapHeight = value;
                RaisePropertyChanged();
            }
        }

        double mapResolution;
        public double MapResolution {
            get { return mapResolution; }
            set {
                mapResolution = value;
                RaisePropertyChanged();
            }
        }
    }
}
