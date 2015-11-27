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
        public static readonly int LEFT_APPROACHING_THRESHOLD = 650;//750;
        public static readonly int LEFT_ON_THRESHOLD = 850;
        public static readonly int RIGHT_APPROACHING_THRESHOLD = 650;//750;
        public static readonly int RIGHT_ON_THRESHOLD = 850;

        public enum SensorLocation
        {
            Front,
            Left,
            Right
        }

        IntersectionType previousTestIntersection;
        IntersectionType currentTestIntersection;
        double lastTick;

        int lastLeftEncoder;
        int lastRightEncoder;

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

        public void HandleIncomingData(int frontSensor, int leftSensor, int rightSensor, int encoderLeft, int encoderRight) {
            IdentifyIntersection(timer.ElapsedMilliseconds, frontSensor, leftSensor, rightSensor, encoderLeft, encoderRight);
        }

        int currentFront;
        int currentLeft;
        int currentRight;

        private void IdentifyIntersection(long tick, int frontSensor, int leftSensor, int rightSensor, int encoderLeft, int encoderRight) {
            currentFront = frontSensor;
            currentLeft = leftSensor;
            currentRight = rightSensor;

            currentTestIntersection = IntersectionType.None;

            if (IsOnOrApproaching(SensorLocation.Left)) {
                currentTestIntersection = IntersectionType.LeftTurn;
            } else if (IsOnOrApproaching(SensorLocation.Right)) {
                currentTestIntersection = IntersectionType.RightTurn;
            }

            if (IsOnOrApproaching(SensorLocation.Front) && IsOnOrApproaching(SensorLocation.Left)) {
                currentTestIntersection = IntersectionType.TLeft;
            } else if (IsOnOrApproaching(SensorLocation.Front) && IsOnOrApproaching(SensorLocation.Right)) {
                currentTestIntersection = IntersectionType.TRight;
            }

            if (IsOnOrApproaching(SensorLocation.Left) && IsOnOrApproaching(SensorLocation.Right)) {
                currentTestIntersection = IntersectionType.T;
            }

            if (IsOnOrApproaching(SensorLocation.Front) && IsOnOrApproaching(SensorLocation.Left) && IsOnOrApproaching(SensorLocation.Right)) {
                currentTestIntersection = IntersectionType.Cross;
            }

            if (Math.Abs(encoderLeft - lastLeftEncoder) > 75) {
                DetectedIntersection = currentTestIntersection;
            } else {
                double currentTick = (encoderLeft + encoderRight) / 2;
                if (currentTestIntersection != previousTestIntersection) {
                    DetectedIntersection = IntersectionType.None;

                    previousTestIntersection = currentTestIntersection;
                    lastTick = currentTick;
                } else {
                    double testTick = lastTick + GetEncoderDistanceTicks();
                    if (currentTick > testTick) {
                        DetectedIntersection = currentTestIntersection;
                        lastTick = currentTick;
                    }
                }
            }

            lastLeftEncoder = encoderLeft;
            lastRightEncoder = encoderRight;
        }

        private double GetEncoderDistanceTicks() {
            double result = Math.Round(100.37 * 6 * (1f / 22));

            return result;
            //return 22 / 3 * 3/ 22;
            //GEAR_RATIO* ENCODER_TEETH_COUNT = 100.37 * 12
        }

        private bool IsOnOrApproaching(SensorLocation sensorLocation) {
            return (IsApproaching(sensorLocation) || IsOn(sensorLocation));
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
