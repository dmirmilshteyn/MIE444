using Microsoft.Graphics.Canvas.UI.Xaml;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
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
        }

        private void CanvasAnimatedControl_PointerPressed(object sender, PointerRoutedEventArgs e) {
            pointerPressed = true;
            if (e.KeyModifiers == Windows.System.VirtualKeyModifiers.Control) {
                deleting = true;
            }

            ProcessPointerMoved(sender, e);
        }

        private void CanvasAnimatedControl_PointerReleased(object sender, PointerRoutedEventArgs e) {
            pointerPressed = false;
            deleting = false;
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
