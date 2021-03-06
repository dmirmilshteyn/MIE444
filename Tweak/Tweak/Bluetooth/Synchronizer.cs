﻿using System;
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

        int stallPWM;
        public int StallPWM {
            get { return stallPWM; }
            set {
                stallPWM = value;
                RaisePropertyChanged();

                SendCommand(BTCommand.StallPWM, stallPWM);
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

        int frontIntersectionSensor;
        public int FrontIntersectionSensor {
            get { return frontIntersectionSensor; }
            set {
                frontIntersectionSensor = value;
                RaisePropertyChanged();
            }
        }

        int leftIntersectionSensor;
        public int LeftIntersectionSensor {
            get { return leftIntersectionSensor; }
            set {
                leftIntersectionSensor = value;
                RaisePropertyChanged();
            }
        }

        int rightIntersectionSensor;
        public int RightIntersectionSensor {
            get { return rightIntersectionSensor; }
            set {
                rightIntersectionSensor = value;
                RaisePropertyChanged();
            }
        }

        int leftLineSensor;
        public int LeftLineSensor {
            get { return leftLineSensor; }
            set {
                leftLineSensor = value;
                RaisePropertyChanged();
            }
        }

        int rightLineSensor;
        public int RightLineSensor {
            get { return rightLineSensor; }
            set {
                rightLineSensor = value;
                RaisePropertyChanged();
            }
        }

        int leftMotorCount;
        public int LeftMotorCount {
            get { return leftMotorCount; }
            set {
                leftMotorCount = value;
                RaisePropertyChanged();
            }
        }

        int rightMotorCount;
        public int RightMotorCount {
            get { return rightMotorCount; }
            set {
                rightMotorCount = value;
                RaisePropertyChanged();
            }
        }

        int intersectionId;
        public int IntersectionId {
            get { return intersectionId; }
            set {
                intersectionId = value;
                RaisePropertyChanged();
            }
        }

        PathPlan pathPlan;
        public PathPlan PathPlan {
            get { return pathPlan; }
            set {
                pathPlan = value;
                RaisePropertyChanged();
            }
        }

        PathPlanNode currentPathPlanNode;
        public PathPlanNode CurrentPathPlanNode {
            get { return currentPathPlanNode; }
            set {
                currentPathPlanNode = value;
                RaisePropertyChanged();
            }
        }

        IntersectionIdentifier intersectionIdentifier;
        public IntersectionIdentifier IntersectionIdentifier {
            get { return intersectionIdentifier; }
            set {
                intersectionIdentifier = value;
                RaisePropertyChanged();
            }
        }

        Task listenTask;

        public Synchronizer(StreamSocket socket) {
            this.socket = socket;
            this.intersectionIdentifier = new IntersectionIdentifier();
            this.PathPlan = new PathPlan();

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
                    case '!': {
                            line = line.Substring(2);

                            string[] resultSegments = line.Split('|');

                            Kp = float.Parse(resultSegments[0]);
                            Ki = float.Parse(resultSegments[1]);
                            Kd = float.Parse(resultSegments[2]);
                            SpeedAdjust = float.Parse(resultSegments[3]);
                            AverageSpeed = Int32.Parse(resultSegments[4]);
                            StallPWM = Int32.Parse(resultSegments[5]);
                        }
                        break;
                    case '?': {
                            line = line.Substring(2);

                            string[] resultSegments = line.Split('|');
                            CurrentError = float.Parse(resultSegments[0]);
                            Integral = float.Parse(resultSegments[1]);
                            Derivative = float.Parse(resultSegments[2]);
                            Controller = float.Parse(resultSegments[3]);
                            LeftSpeed = float.Parse(resultSegments[4]);
                            RightSpeed = float.Parse(resultSegments[5]);

                            LeftLineSensor = (int)float.Parse(resultSegments[6]);
                            RightLineSensor = (int)float.Parse(resultSegments[7]);
                        }
                        break;
                    case '#': {
                            line = line.Substring(2);

                            string[] resultSegments = line.Split('|');
                            FrontIntersectionSensor = (int)float.Parse(resultSegments[0]);
                            LeftIntersectionSensor = (int)float.Parse(resultSegments[1]);
                            RightIntersectionSensor = (int)float.Parse(resultSegments[2]);

                            // TODO: Remove this! Support for older version of arduino code
                            if (resultSegments.Length == 4) {
                                IntersectionIdentifier.DetectedIntersection = (IntersectionType)(int)float.Parse(resultSegments[3]);
                            }

                            //if (!IntersectionIdentifier.IsRunning) {
                            //    IntersectionIdentifier.Start();
                            //}

                            //IntersectionIdentifier.HandleIncomingData(frontIntersectionSensor, LeftIntersectionSensor, RightIntersectionSensor, LeftMotorCount, RightMotorCount);
                        }
                        break;
                    case '*': {
                            line = line.Substring(2);

                            string[] resultSegments = line.Split('|');
                            int width = (int)float.Parse(resultSegments[0]);
                            int height = (int)float.Parse(resultSegments[1]);

                            bool[,] map = new bool[width, height];
                            for (int x = 0; x < width; x++) {
                                for (int y = 0; y < height; y++) {
                                    map[x, y] = bool.Parse(resultSegments[2 + (y * width + x)]);
                                }
                            }
                        }
                        break;
                    case '$': {
                            line = line.Substring(2);

                            string[] resultSegments = line.Split('|');
                            LeftMotorCount = (int)float.Parse(resultSegments[0]);
                            RightMotorCount = (int)float.Parse(resultSegments[1]);
                        }
                        break;
                    case '&': {
                            line = line.Substring(2);

                            string[] resultSegments = line.Split('|');
                            float relativePositionX = float.Parse(resultSegments[0]);
                            float relativePositionY = float.Parse(resultSegments[1]);
                            IntersectionId = (int)float.Parse(resultSegments[2]);

                            System.Diagnostics.Debug.WriteLine($"{relativePositionX} {relativePositionY}");
                        }
                        break;
                    case '^': {
                            line = line.Substring(2);

                            string[] resultSegments = line.Split('|');
                            int size = (int)float.Parse(resultSegments[0]);

                            int offset = 1;
                            PathPlan.Plan.Clear();
                            for (int i = 0; i < size; i++) {
                                int markerId = (int)float.Parse(resultSegments[offset++]);
                                int intersectionid = (int)float.Parse(resultSegments[offset++]);
                                TurnDirection turnDirection = (TurnDirection)(int)float.Parse(resultSegments[offset++]);

                                PathPlan.Plan.Add(new PathPlanNode()
                                {
                                    MarkerId = markerId,
                                    IntersectionId = intersectionid,
                                    TurnDirection = turnDirection
                                });


                            }
                        }
                        break;
                    case '+': {
                            line = line.Substring(2);

                            string[] resultSegments = line.Split('|');
                            int currentPathPlanNodeIndex = (int)float.Parse(resultSegments[0]);

                            if (currentPathPlanNodeIndex != -1 && PathPlan.Plan.Count > currentPathPlanNodeIndex) {
                                CurrentPathPlanNode = PathPlan.Plan[currentPathPlanNodeIndex];
                            }
                        }
                        break;
                }
            } else {
                System.Diagnostics.Debug.WriteLine(line);
                //OutputLogString += line;
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


        // TODO: Remove this! Encoder testing code
        //int previousLeftMotorCount = 0;
        //int previousRightMotorCount = 0;
        //double relativeHeadingAngle = 0;
        //int relativeLocationX = 0;
        //int relativeLocationY = 0;
        //double relativeLocationXMeters = 0;
        //double relativeLocationYMeters = 0;

        //private void updateRelativeLocation() {
        //    int leftEncoderDiff = leftMotorCount - previousLeftMotorCount;
        //    previousLeftMotorCount += leftEncoderDiff;
        //    int rightEncoderDiff = rightMotorCount - previousRightMotorCount;
        //    previousRightMotorCount += rightEncoderDiff;

        //    double relativeHeadingAngleDiff;
        //    double travelDist;

        //    double leftEncoderDiffMeters = leftEncoderDiff / 6 / 100.37 * 0.035 * 2 * 3.14159265358979323846;
        //    double rightEncoderDiffMeters = rightEncoderDiff / 6 / 100.37 * 0.035 * 2 * 3.14159265358979323846;

        //    relativeHeadingAngleDiff = (rightEncoderDiffMeters - leftEncoderDiffMeters) / 0.28;

        //    if (Math.Abs(relativeHeadingAngleDiff) > 0.001) {
        //        travelDist = Math.Abs(((leftEncoderDiffMeters + rightEncoderDiffMeters) * Math.Sqrt(2 - 2 * Math.Cos(relativeHeadingAngleDiff))) / (2 * relativeHeadingAngleDiff));
        //    } else {
        //        travelDist = Math.Abs(((leftEncoderDiffMeters + rightEncoderDiffMeters) / (2)));
        //    }

        //    relativeLocationXMeters += Math.Cos(relativeHeadingAngle + relativeHeadingAngleDiff / 2) * travelDist;
        //    relativeLocationYMeters += Math.Sin(relativeHeadingAngle + relativeHeadingAngleDiff / 2) * travelDist;

        //    relativeLocationX = (int)Math.Round(relativeLocationXMeters / 0.025);
        //    relativeLocationY = (int)Math.Round(relativeLocationYMeters / 0.025);

        //    relativeHeadingAngle += relativeHeadingAngleDiff;
        //}
    }
}
