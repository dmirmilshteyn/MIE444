using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tweak
{
    /// <summary>
    /// Used for debugging to quickly determine an ideal algorithm for identifying intersections
    /// </summary>
    class IntersectionIdentifier : ObservableObject
    {
        public static readonly int FRONT_APPROACHING_THRESHOLD = 750;
        public static readonly int FRONT_ON_THRESHOLD = 850;
        public static readonly int LEFT_APPROACHING_THRESHOLD = 750;
        public static readonly int LEFT_ON_THRESHOLD = 900;
        public static readonly int RIGHT_APPROACHING_THRESHOLD = 750;
        public static readonly int RIGHT_ON_THRESHOLD = 900;

        public enum SensorLocation
        {
            Front,
            Left,
            Right
        }

        IntersectionType intersection;
        public IntersectionType DetectedIntersection {
            get { return intersection; }
            set {
                intersection = value;
                RaisePropertyChanged();
            }
        }

        public bool IsRunning {
            get { return timer.IsRunning; }
        }

        Stopwatch timer;

        public IntersectionIdentifier() {
            timer = new Stopwatch();

            this.DetectedIntersection = IntersectionType.None;
        }

        public void Start() {
            timer.Reset();
            timer.Start();
        }

        public void Stop() {
            timer.Stop();
        }

        public void HandleIncomingData(int frontSensor, int leftSensor, int rightSensor) {
            IdentifyIntersection(timer.ElapsedMilliseconds, frontSensor, leftSensor, rightSensor);
        }

        int currentFront;
        int currentLeft;
        int currentRight;

        private void IdentifyIntersection(long tick, int frontSensor, int leftSensor, int rightSensor) {
            currentFront = frontSensor;
            currentLeft = leftSensor;
            currentRight = rightSensor;

            DetectedIntersection = IntersectionType.None;

            if (IsOn(SensorLocation.Left)) {
                DetectedIntersection = IntersectionType.LeftTurn;
            }

            if (IsOn(SensorLocation.Right)) {
                DetectedIntersection = IntersectionType.RightTurn;
            }

            if (IsOn(SensorLocation.Front) && IsOn(SensorLocation.Left)) {
                DetectedIntersection = IntersectionType.TLeft;
            }

            if (IsOn(SensorLocation.Front) && IsOn(SensorLocation.Right)) {
                DetectedIntersection = IntersectionType.TRight;
            }

            if (IsOn(SensorLocation.Left) && IsOn(SensorLocation.Right)) {
                DetectedIntersection = IntersectionType.T;
            }

            if (IsOn(SensorLocation.Front) && IsOn(SensorLocation.Left) && IsOn(SensorLocation.Right)) {
                DetectedIntersection = IntersectionType.Cross;
            }
        }

        private bool IsApproaching(SensorLocation sensorLocation) {
            int approachingThreshold = GetApproachingThreshold(sensorLocation);
            int onThreshold = GetOnThreshold(sensorLocation);
            int value = GetCurrentValue(sensorLocation);

            return (value > approachingThreshold && value < onThreshold);
        }

        private bool IsOn(SensorLocation sensorLocation) {
            int onThreshold = GetOnThreshold(sensorLocation);
            int value = GetCurrentValue(sensorLocation);

            return (value >= onThreshold);
        }

        private int GetCurrentValue(SensorLocation sensorLocation) {
            switch (sensorLocation) {
                case SensorLocation.Front:
                    return currentFront;
                case SensorLocation.Left:
                    return currentLeft;
                case SensorLocation.Right:
                    return currentRight;
            }

            throw new NotSupportedException();
        }

        private int GetOnThreshold(SensorLocation sensorLocation) {
            switch (sensorLocation) {
                case SensorLocation.Front:
                    return FRONT_ON_THRESHOLD;
                case SensorLocation.Left:
                    return LEFT_ON_THRESHOLD;
                case SensorLocation.Right:
                    return RIGHT_ON_THRESHOLD;
            }

            throw new NotSupportedException();
        }

        private int GetApproachingThreshold(SensorLocation sensorLocation) {
            switch (sensorLocation) {
                case SensorLocation.Front:
                    return FRONT_APPROACHING_THRESHOLD;
                case SensorLocation.Left:
                    return LEFT_APPROACHING_THRESHOLD;
                case SensorLocation.Right:
                    return RIGHT_APPROACHING_THRESHOLD;
            }

            throw new NotSupportedException();
        }


    }
}
