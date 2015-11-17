using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Windows.Networking.Sockets;
using Windows.Storage.Streams;

namespace Tweak.Bluetooth
{
    class Synchronizer : ObservableObject
    {
        StreamSocket socket;
        DataWriter dataWriter;
        DataReader dataReader;

        float kp;
        public float Kp {
            get { return kp; }
            set {
                kp = value;
                RaisePropertyChanged();

                SendCommand(BTCommand.SetP, kp);
            }
        }

        float ki;
        public float Ki {
            get { return ki; }
            set {
                ki = value;
                RaisePropertyChanged();

                SendCommand(BTCommand.SetI, ki);
            }
        }

        float kd;
        public float Kd {
            get { return kd; }
            set {
                kd = value;
                RaisePropertyChanged();

                SendCommand(BTCommand.SetD, kd);
            }
        }

        float speedAdjust;
        public float SpeedAdjust {
            get { return speedAdjust; }
            set {
                speedAdjust = value;
                RaisePropertyChanged();

                SendCommand(BTCommand.SpeedAdjust, speedAdjust);
            }
        }

        int averageSpeed;
        public int AverageSpeed {
            get { return averageSpeed; }
            set {
                averageSpeed = value;
                RaisePropertyChanged();

                SendCommand(BTCommand.AverageSpeed, averageSpeed);
            }
        }

        string outputLogString;
        public string OutputLogString {
            get { return outputLogString; }
            set {
                outputLogString = value;
                RaisePropertyChanged();
            }
        }

        float currentError;
        public float CurrentError {
            get { return currentError; }
            set {
                currentError = value;
                RaisePropertyChanged();
            }
        }

        float integral;
        public float Integral {
            get { return integral; }
            set {
                integral = value;
                RaisePropertyChanged();
            }
        }

        float derivative;
        public float Derivative {
            get { return derivative; }
            set {
                derivative = value;
                RaisePropertyChanged();
            }
        }

        float controller;
        public float Controller {
            get { return controller; }
            set {
                controller = value;
                RaisePropertyChanged();
            }
        }

        float leftSpeed;
        public float LeftSpeed {
            get { return leftSpeed; }
            set {
                leftSpeed = value;
                RaisePropertyChanged();
            }
        }

        float rightSpeed;
        public float RightSpeed {
            get { return rightSpeed; }
            set {
                rightSpeed = value;
                RaisePropertyChanged();
            }
        }

        Task listenTask;

        public Synchronizer(StreamSocket socket) {
            this.socket = socket;

            this.dataWriter = new DataWriter(socket.OutputStream);
            this.dataReader = new DataReader(socket.InputStream);

            listenTask = StartListeningAsync(dataReader);
        }

        public async Task Send(string data) {
            dataWriter.WriteString(data);

            await dataWriter.StoreAsync();
        }

        public async Task Synchronize() {
            dataWriter.WriteString((char)BTCommand.RequestSync + "|");

            await dataWriter.StoreAsync();
        }

        private void HandleLine(string line) {
            if (line.StartsWith("!")) {
                switch (line[1]) {
                    case '!':
                        {
                            line = line.Substring(2);

                            string[] resultSegments = line.Split('|');

                            Kp = float.Parse(resultSegments[0]);
                            Ki = float.Parse(resultSegments[1]);
                            Kd = float.Parse(resultSegments[2]);
                            SpeedAdjust = float.Parse(resultSegments[3]);
                            AverageSpeed = Int32.Parse(resultSegments[4]);
                        }
                        break;
                    case '?':
                        {
                            line = line.Substring(2);

                            string[] resultSegments = line.Split('|');
                            CurrentError = float.Parse(resultSegments[0]);
                            Integral = float.Parse(resultSegments[1]);
                            Derivative = float.Parse(resultSegments[2]);
                            Controller = float.Parse(resultSegments[3]);
                            LeftSpeed = float.Parse(resultSegments[4]);
                            RightSpeed = float.Parse(resultSegments[5]);
                        }
                        break;
                }
            } else {
                OutputLogString += line;
            }
        }

        private void SendCommand(BTCommand command, float payload) {
            SendCommand(command, payload.ToString());
        }

        private async void SendCommand(BTCommand command, string payload) {
            string fullPayload = (char)command + payload + "|";

            dataWriter.WriteString(fullPayload);

            await dataWriter.StoreAsync();
        }

        public async Task StartListeningAsync(DataReader dataReader) {
            while (true) {
                StringBuilder builder = new StringBuilder();
                char letter;
                do {
                    var readOperation = await dataReader.LoadAsync(1);
                    letter = (char)this.dataReader.ReadByte();

                    builder.Append(letter);
                } while (letter != '\n');

                HandleLine(builder.ToString());
            }
        }
    }
}
