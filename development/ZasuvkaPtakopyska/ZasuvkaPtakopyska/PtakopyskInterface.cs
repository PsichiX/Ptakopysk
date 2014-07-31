using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace ZasuvkaPtakopyska
{
    public class PtakopyskInterface
    {
        private const string DLL = "PtakopyskInterface.dll";

        [DllImport(DLL)]
        public static extern void Initialize(IntPtr handle);

        [DllImport(DLL)]
        public static extern void Release();

        [DllImport(DLL)]
        public static extern void ProcessEvents();

        [DllImport(DLL)]
        public static extern void ProcessPhysics(float deltaTime, int velocityIterations, int positionIterations);

        [DllImport(DLL)]
        public static extern void ProcessUpdate(float deltaTime, bool sortInstances);

        [DllImport(DLL)]
        public static extern void ProcessRender();

        [DllImport(DLL)]
        public static extern void SetVerticalSyncEnabled(bool enabled);
    }
}
