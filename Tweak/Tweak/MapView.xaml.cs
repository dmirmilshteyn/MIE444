using Microsoft.Graphics.Canvas.UI.Xaml;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
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
    public sealed partial class MapView : UserControl
    {
        bool pointerPressed;
        bool deleting;
        Object sharedDataContext;

        Point? intersectionA;
        Point? intersectionB;

        Point? pathPlanningA;
        Point? pathPlanningB;

        IReadOnlyList<Position> path;

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

        private void CanvasAnimatedControl_PointerPressed(object sender, PointerRoutedEventArgs e) {
            Project project = (Project)sharedDataContext;
            Map map = project.Map;

            pointerPressed = true;
            if (e.KeyModifiers == Windows.System.VirtualKeyModifiers.Control) {
                deleting = true;
            }
            if (e.KeyModifiers == Windows.System.VirtualKeyModifiers.Shift) {
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

                    Intersection intersection = new Intersection(intersectionA.Value.X, intersectionA.Value.Y, width, height);

                    map.Intersections.Add(intersection);

                    intersectionA = null;
                    intersectionB = null;
                }
            }
            if (e.KeyModifiers == Windows.System.VirtualKeyModifiers.Windows) {
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

            ProcessPointerMoved(sender, e);
        }

        private void CanvasAnimatedControl_PointerReleased(object sender, PointerRoutedEventArgs e) {
            Project project = (Project)sharedDataContext;
            Map map = project.Map;

            pointerPressed = false;
            deleting = false;

            if (e.KeyModifiers == Windows.System.VirtualKeyModifiers.Control) {
                if (pathPlanningA.HasValue && pathPlanningB.HasValue) {
                    Pathfinder pathfinder = new Pathfinder(map);
                    path = pathfinder.AStar(new Position((int)pathPlanningA.Value.X, (int)pathPlanningA.Value.Y), new Position((int)pathPlanningB.Value.X, (int)pathPlanningB.Value.Y));
                }
            }
        }

        private void ProcessPointerMoved(object sender, PointerRoutedEventArgs e) {
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
