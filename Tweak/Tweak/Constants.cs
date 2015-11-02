using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tweak
{
    public class Constants : ObservableObject
    {
        int ain1;
        public int AIN1 {
            get { return ain1; }
            set {
                ain1 = value;
                RaisePropertyChanged();
            }
        }

        int ain2;
        public int AIN2 {
            get { return ain2; }
            set {
                ain2 = value;
                RaisePropertyChanged();
            }
        }

        int bin1;
        public int BIN1 {
            get { return bin1; }
            set {
                bin1 = value;
                RaisePropertyChanged();
            }
        }

        int bin2;
        public int BIN2 {
            get { return bin2; }
            set {
                bin2 = value;
                RaisePropertyChanged();
            }
        }

        double wheelRadius;
        public double WheelRadius {
            get { return wheelRadius; }
            set {
                wheelRadius = value;
                RaisePropertyChanged();
            }
        }

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
