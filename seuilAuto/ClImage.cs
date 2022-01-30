﻿using System;
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
        public static extern IntPtr traitementTest(int nbChamps, IntPtr data, int stride, int nbLig, int nbCol, double[] parametres, int nbChamps_p, IntPtr data_p, int stride_p, int nbLig_p, int nbCol_p);

        public IntPtr traitementTestPtr(int nbChamps, IntPtr data, int stride, int nbLig, int nbCol, double[] parametres, int nbChamps_p, IntPtr data_p, int stride_p, int nbLig_p, int nbCol_p)
        {
            // nbChamps, data, stride, nbLig, NbCol, parametres : correspondent à l'image puzzle de référence
            // les autres paramètres concernent l'image piece puzzle à détecter
            ClPtr = traitementTest(nbChamps, data, stride, nbLig, nbCol, parametres, nbChamps_p, data_p, stride_p, nbLig_p, nbCol_p);
            return ClPtr;
        }

        //Appel du traitement par PattermMatching
        [DllImport("Traitement.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr PatternMatching(IntPtr data, int stride, int nbLig, int nbCol, IntPtr data_p, int stride_p, int nbLig_p, int nbCol_p);

        public IntPtr PatternMatchingPtr(IntPtr data, int stride, int nbLig, int nbCol, IntPtr data_p, int stride_p, int nbLig_p, int nbCol_p)
        {
            // nbChamps, data, stride, nbLig, NbCol, parametres : correspondent à l'image puzzle de référence
            // les autres paramètres concernent l'image piece puzzle à détecter
            ClPtr = PatternMatching(data, stride, nbLig, nbCol, data_p, stride_p, nbLig_p, nbCol_p);
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
