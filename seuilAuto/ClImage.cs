using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Runtime.InteropServices;
using System.Drawing;

namespace Traitement
{
    public class ClImage
    {
        // on crée une classe C# avec pointeur sur l'objet C++
        // puis des static extern exportées de chaque méthode utile de la classe C++
        public IntPtr ClPtr;

        public ClImage()
        {
            ClPtr = IntPtr.Zero;
        }

        ~ClImage()
        {
            if (ClPtr != IntPtr.Zero)
                ClPtr = IntPtr.Zero;
        }


        // va-et-vient avec constructeur C#/C++
        // obligatoire dans toute nouvelle classe propre à l'application
        [DllImport("Traitement.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr objetLib();

        public IntPtr objetLibPtr()
        {
            ClPtr = objetLib();
            return ClPtr;
        }

        // Appel de la méthode pour le traitement par défaut
        [DllImport("Traitement.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr objetLibDataImg(int nbChamps, IntPtr data, int stride, int nbLig, int nbCol);

        public IntPtr objetLibDataImgPtr(int nbChamps, IntPtr data, int stride, int nbLig, int nbCol)
        {
            ClPtr = objetLibDataImg(nbChamps,data, stride, nbLig, nbCol);
            return ClPtr;
        }

        //Appel de la méthode de traitement test

        [DllImport("Traitement.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr traitementTest(int nbChamps, IntPtr data, int stride, int nbLig, int nbCol, double[] parametres);

        public IntPtr traitementTestPtr(int nbChamps, IntPtr data, int stride, int nbLig, int nbCol, double[] parametres)
        {
            ClPtr = traitementTest(nbChamps, data, stride, nbLig, nbCol, parametres);
            return ClPtr;
        }
        // Appel de la méthode pour retrouver les signatures
        [DllImport("Traitement.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern double valeurChamp(IntPtr pImg, int i);

        public double objetLibValeurChamp(int i)
        {
            return valeurChamp(ClPtr, i);
        }
    }
}
