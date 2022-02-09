// ajouter _declspec(dllexport) devant tout public pour permettre à la dll d'exporter ces méthodes 
// pour qu'elles puissent être utilisées par d'autres applications ou programmes

#pragma once

#ifndef _IMAGE_DOUBLE_
#define _IMAGE_DOUBLE_

// définition classe Image Double pour stocker informations autres que ndg
#include <vector>
#include <string>

#include "ImageNdg.h"

typedef struct pics {
	int numero;
	double angles;
	double rhos;
	double taille;
} PICS;

class CImageDouble {

	///////////////////////////////////////
	private : 
	///////////////////////////////////////

		int              m_iHauteur;
		int              m_iLargeur; 
		std::string      m_sNom;
		double			 m_vMin; // valeur plus petite possible
		double           m_vMax; // valeur plus grande possible
		double*          m_pucPixel;

	///////////////////////////////////////
	public : 
	///////////////////////////////////////

		// constructeurs
		_declspec(dllexport) CImageDouble(); // par défaut
		_declspec(dllexport) CImageDouble(int hauteur, int largeur, double valeur = 0.0); // initialisation (défaut 0)
		_declspec(dllexport) CImageDouble(const CImageNdg& im, const std::string& methode = "normalise");  // choix "normalise"/"cast"/"integrale1"/"integrale2"
		// image Ndg en entrée
		_declspec(dllexport) CImageDouble(const CImageDouble& im); // image en entrée

		// destructeur
		_declspec(dllexport) ~CImageDouble();

		// pouvoir accéder à un pixel par image(i)
		_declspec(dllexport) double& operator() (int i) const { 
		return m_pucPixel[i];
		}

		// pouvoir accéder à un pixel par image(i,j)
		_declspec(dllexport) double& operator() (int i, int j) const { 
		return m_pucPixel[i*m_iLargeur+j];
		}

		// opérateur copie image par imOut = imIn
		_declspec(dllexport) CImageDouble& operator=(const CImageDouble& im);

		// get et set 

		_declspec(dllexport) int lireHauteur() const {  
		return m_iHauteur;
		}

		_declspec(dllexport) int lireLargeur() const {
		return m_iLargeur;
		}

		_declspec(dllexport) double lireMin() const {
			return m_vMin;
		}

		_declspec(dllexport) double lireMax() const {
			return m_vMax;
		}

		_declspec(dllexport) std::string lireNom() const {
		return m_sNom;
		}

		_declspec(dllexport) void ecrireHauteur(int hauteur) {
		m_iHauteur = hauteur;
		}

		_declspec(dllexport) void ecrireLargeur(int largeur) {
		m_iLargeur = largeur;
		}

		_declspec(dllexport) void ecrireMin(double min) {
			m_vMin = min;
		}

		_declspec(dllexport) void ecrireMax(double max) {
			m_vMax = max;
		}

		_declspec(dllexport) void ecrireNom(std::string nom) {
		m_sNom = nom;
		}

		_declspec(dllexport) int lireNbPixels() const { 
		return m_iHauteur*m_iLargeur;
		}

		// seuillage manuel uniquement
		_declspec(dllexport) CImageDouble seuillage(double seuilBas = 0, double seuilHaut = 1);

		// distance au fond
		_declspec(dllexport) CImageDouble distance(std::string eltStructurant = "V8", double valBord = 0);

		// conversions !!!
		_declspec(dllexport) CImageNdg toNdg(const std::string& methode = "defaut"); // choix "defaut"/"expansion"

		// Hough Transform
		_declspec(dllexport) CImageDouble planHough();
		_declspec(dllexport) CImageDouble maxiLocaux(int N = 9, int M = 9) const;
		_declspec(dllexport) CImageDouble extractionLignes(int N=9, int M=9, double dimLigne=50, bool affichage="true");
		_declspec(dllexport) CImageNdg    houghInverse(const CImageNdg& img); 

		// Vecteur gradient
		_declspec(dllexport) CImageDouble vecteurGradient(const std::string& axe = "norme"); // choix "norme"/"angle"

		// Filtrage : moyen/gaussien
		_declspec(dllexport) CImageDouble filtrage(const std::string & methode, int N, double sigma);

		_declspec(dllexport) std::vector<CImageDouble> pyramide(int hauteur, int tailleFiltre, double sigma);

		//image avec lignes de hough détectées
		_declspec(dllexport) CImageNdg houghExtractionLignes(const CImageNdg & img, const std::string & methode, int N, int M, int dim, int nombre, bool enregistrementCSV);

		// lignes de hough
		_declspec(dllexport) std::vector<PICS> houghLignes(const CImageNdg & img, int N, int M, int dim, int nombre = 1);
		//moyenne de l'image
		_declspec(dllexport) double moyenne() const;

		//convolution de deux images
		_declspec(dllexport) CImageDouble conv(const CImageDouble& kernel);
		
		// convolution avec fft, n'est important que pour avoir un temps de traitement plus raisonnable
		_declspec(dllexport) CImageDouble convfft(const CImageDouble & kernel);

		//corrélationNormée entre deux images
		CImageDouble NormCorr(const CImageDouble & scene);

};

#endif 