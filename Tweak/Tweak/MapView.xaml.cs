using Microsoft.Graphics.Canvas.Text;
using Microsoft.Graphics.Canvas.UI.Xaml;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices.WindowsRuntime;
using Tweak.Pathfinding;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI;
using Windows.UI.Input;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

namespace Tweak
{
    public sealed partial class MapView : UserControl, INotifyPropertyChanged
    {
        bool pointerPressed;
        bool deleting;
        Object sharedDataContext;

        Point? intersectionA;
        Point? intersectionB;

        Point? pathPlanningA;
        Point? pathPlanningB;

        IReadOnlyList<Position> path;

        MapPlacementMode mapPlacementMode;
        public MapPlacementMode MapPlacementMode {
            get { return mapPlacementMode; }
            set {
                mapPlacementMode = value;
                RaisePropertyChanged();
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        public void RaisePropertyChanged([CallerMemberName] string property = "") {
            if (PropertyChanged != null) {
                PropertyChanged(this, new PropertyChangedEventArgs(property));
            }
        }

        public MapView() {
            this.InitializeComponent();

            sharedDataContext = this.DataContext;
            this.DataContextChanged += MapView_DataContextChanged;
        }

        private void MapView_DataContextChanged(FrameworkElement sender, DataContextChangedEventArgs args) {
            sharedDataContext = args.NewValue;
        }

        private void CanvasAnimatedControl_PointerMoved(object sender, PointerRoutedEventArgs e) {
            ProcessPointerMoved(sender, e);
        }

        private void CanvasAnimatedControl_Draw(ICanvasAnimatedControl sender, CanvasAnimatedDrawEventArgs args) {
            Project project = (Project)sharedDataContext;
            Map map = project.Map;

            args.DrawingSession.Antialiasing = Microsoft.Graphics.Canvas.CanvasAntialiasing.Aliased;

            for (int x = 0; x < map.Tiles.Width; x++) {
                for (int y = 0; y < map.Tiles.Height; y++) {
                    if (map.Tiles[x, y].Filled) {
                        args.DrawingSession.DrawRectangle(new Rect(x, y, project.Constants.MapResolution, project.Constants.MapResolution), Colors.Black);
                    } else {
                        args.DrawingSession.DrawRectangle(new Rect(x, y, project.Constants.MapResolution, project.Constants.MapResolution), Colors.White);
                    }
                }
            }

            foreach (Intersection intersection in map.Intersections) {
                args.DrawingSession.DrawRectangle(new Rect(intersection.X, intersection.Y, intersection.Width, intersection.Height), Colors.Green);
            }

            for (int i = 0; i < map.IntersectionMarkers.Count; i++) {
                IntersectionMarker intersectionMarker = map.IntersectionMarkers[i];
                args.DrawingSession.DrawLine(intersectionMarker.X1, intersectionMarker.Y1, intersectionMarker.X2, intersectionMarker.Y2, Colors.Green, 1);

                args.DrawingSession.DrawText(intersectionMarker.IntersectionId.ToString(), intersectionMarker.X1, intersectionMarker.Y1, Colors.Red, new CanvasTextFormat() { FontSize = 4 });
            }

            for (int i = 0; i < map.StartPositions.Count; i++) {
                StartPosition startPosition = map.StartPositions[i];

                args.DrawingSession.DrawRectangle(new Rect(startPosition.X, startPosition.Y, 1, 1), Colors.Purple);
            }

            if (intersectionA.HasValue) {
                args.DrawingSession.DrawRectangle(new Rect(intersectionA.Value.X, intersectionA.Value.Y, 1, 1), Colors.Green);
            }

            if (intersectionB.HasValue) {
                args.DrawingSession.DrawRectangle(new Rect(intersectionB.Value.X, intersectionB.Value.Y, 1, 1), Colors.Green);
            }

            if (pathPlanningA.HasValue) {
                args.DrawingSession.DrawRectangle(new Rect(pathPlanningA.Value.X, pathPlanningA.Value.Y, 1, 1), Colors.Red);
            }
            if (pathPlanningB.HasValue) {
                args.DrawingSession.DrawRectangle(new Rect(pathPlanningB.Value.X, pathPlanningB.Value.Y, 1, 1), Colors.Red);
            }

            if (path != null) {
                foreach (var position in path) {
                    args.DrawingSession.DrawRectangle(new Rect(position.X, position.Y, 1, 1), Colors.Yellow);
                }
            }
        }

        private async void CanvasAnimatedControl_PointerPressed(object sender, PointerRoutedEventArgs e) {
            Project project = (Project)sharedDataContext;
            Map map = project.Map;

            pointerPressed = true;
            switch (MapPlacementMode) {
                case MapPlacementMode.Tiles:
                    {
                        if (e.KeyModifiers == Windows.System.VirtualKeyModifiers.Control) {
                            deleting = true;
                        }

                        ProcessPointerMoved(sender, e);
                    }
                    break;
                case MapPlacementMode.Intersections:
                    {
                        pointerPressed = false;
                        if (!intersectionA.HasValue) {
                            UIElement canvas = (UIElement)sender;

                            PointerPoint point = e.GetCurrentPoint(canvas);

                            intersectionA = point.Position;
                        } else if (!intersectionB.HasValue) {
                            UIElement canvas = (UIElement)sender;

                            PointerPoint point = e.GetCurrentPoint(canvas);
                            intersectionB = point.Position;

                            double width = intersectionB.Value.X - intersectionA.Value.X;
                            double height = intersectionB.Value.Y - intersectionA.Value.Y;

                            IntersectionMarker marker = new IntersectionMarker();
                            IntersectionMarkerConfiguration configurationDialog = new IntersectionMarkerConfiguration();
                            configurationDialog.DataContext = marker;

                            marker.X1 = (int)intersectionA.Value.X;
                            marker.Y1 = (int)intersectionA.Value.Y;
                            marker.X2 = (int)intersectionB.Value.X;
                            marker.Y2 = (int)intersectionB.Value.Y;

                            var result = await configurationDialog.ShowAsync();
                            if (result == ContentDialogResult.Primary) {
                                map.IntersectionMarkers.Add(marker);
                            }

                            //Intersection intersection = new Intersection(intersectionA.Value.X, intersectionA.Value.Y, width, height);

                            //map.Intersections.Add(intersection);

                            intersectionA = null;
                            intersectionB = null;
                        }
                    }
                    break;
                case MapPlacementMode.Path:
                    {
                        if (!pathPlanningA.HasValue) {
                            UIElement canvas = (UIElement)sender;

                            PointerPoint point = e.GetCurrentPoint(canvas);

                            pathPlanningA = point.Position;
                        } else if (!pathPlanningB.HasValue) {
                            UIElement canvas = (UIElement)sender;

                            PointerPoint point = e.GetCurrentPoint(canvas);

                            pathPlanningB = point.Position;
                        }
                    }
                    break;
                case MapPlacementMode.StartPositions:
                    {
                        if (e.KeyModifiers != Windows.System.VirtualKeyModifiers.Control) {
                            UIElement canvas = (UIElement)sender;

                            PointerPoint point = e.GetCurrentPoint(canvas);

                            StartPosition startPosition = new StartPosition();
                            startPosition.X = (int)Math.Round(point.Position.X);
                            startPosition.Y = (int)Math.Round(point.Position.Y);

                            map.StartPositions.Add(startPosition);
                        }
                    }
                    break;
            }
        }

        private void CanvasAnimatedControl_PointerReleased(object sender, PointerRoutedEventArgs e) {
            Project project = (Project)sharedDataContext;
            Map map = project.Map;

            pointerPressed = false;
            deleting = false;

            switch (MapPlacementMode) {
                case MapPlacementMode.Path:
                    {
                        if (e.KeyModifiers == Windows.System.VirtualKeyModifiers.Control) {
                            if (pathPlanningA.HasValue && pathPlanningB.HasValue) {
                                AStarPathfinder pathfinder = new AStarPathfinder(map);
                                path = pathfinder.AStar(new Position((int)pathPlanningA.Value.X, (int)pathPlanningA.Value.Y), new Position((int)pathPlanningB.Value.X, (int)pathPlanningB.Value.Y));
                            }
                        } else if (e.KeyModifiers.HasFlag(Windows.System.VirtualKeyModifiers.Shift) && e.KeyModifiers.HasFlag(Windows.System.VirtualKeyModifiers.Control)) {
                            pathPlanningA = null;
                            pathPlanningB = null;
                            path = null;
                        }
                    }
                    break;
                case MapPlacementMode.StartPositions:
                    {
                        if (e.KeyModifiers == Windows.System.VirtualKeyModifiers.Control) {
                            map.StartPositions.RemoveAt(map.StartPositions.Count - 1);
                        }
                    }
                    break;
            }
            
        }

        private void ProcessPointerMoved(object sender, PointerRoutedEventArgs e) {
            if (MapPlacementMode == MapPlacementMode.Tiles) {
                Project project = (Project)sharedDataContext;
                Map map = project.Map;

                UIElement canvas = (UIElement)sender;

                PointerPoint point = e.GetCurrentPoint(canvas);

                int x = (int)point.Position.X;
                int y = (int)point.Position.Y;

                if (pointerPressed) {
                    if (!deleting) {
                        map.Tiles[x, y].Filled = true;
                    } else {
                        map.Tiles[x, y].Filled = false;
                    }
                }
            }
        }
    }
}
