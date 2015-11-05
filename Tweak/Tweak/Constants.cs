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

        int lineSensor1Pin;
        public int LineSensor1Pin {
            get { return lineSensor1Pin; }
            set {
                lineSensor1Pin = value;
                RaisePropertyChanged();
            }
        }

        int lineSensor2Pin;
        public int LineSensor2Pin {
            get { return lineSensor2Pin; }
            set {
                lineSensor2Pin = value;
                RaisePropertyChanged();
            }
        }

        int lineSensor3Pin;
        public int LineSensor3Pin {
            get { return lineSensor3Pin; }
            set {
                lineSensor3Pin = value;
                RaisePropertyChanged();
            }
        }

        int lineSensor4Pin;
        public int LineSensor4Pin {
            get { return lineSensor4Pin; }
            set {
                lineSensor4Pin = value;
                RaisePropertyChanged();
            }
        }

        int lineSensor5Pin;
        public int LineSensor5Pin {
            get { return lineSensor5Pin; }
            set {
                lineSensor5Pin = value;
                RaisePropertyChanged();
            }
        }

        int wallSensorPin;
        public int WallSensorPin {
            get { return wallSensorPin; }
            set {
                wallSensorPin = value;
                RaisePropertyChanged();
            }
        }

        int irDetectorPin;
        public int IRDetectorPin {
            get { return irDetectorPin; }
            set {
                irDetectorPin = value;
                RaisePropertyChanged();
            }
        }

        int antennaLEDPin;
        public int AntennaLEDPin {
            get { return antennaLEDPin; }
            set {
                antennaLEDPin = value;
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
