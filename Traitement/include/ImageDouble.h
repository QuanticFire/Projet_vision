// ajouter _declspec(dllexport) devant tout public pour permettre � la dll d'exporter ces m�thodes 
// pour qu'elles puissent �tre utilis�es par d'autres applications ou programmes

#pragma once

#ifndef _IMAGE_DOUBLE_
#define _IMAGE_DOUBLE_

// d�finition classe Image Double pour stocker informations autres que ndg
#include <vector>
#include <string>

#include "ImageNdg.h"

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
		_declspec(dllexport) CImageDouble(); // par d�faut
		_declspec(dllexport) CImageDouble(int hauteur, int largeur); // initialisation � 0
		_declspec(dllexport) CImageDouble(const CImageNdg& im, const std::string& methode = "normalise");  // choix "normalise"/"cast"/"integrale1"/"integrale2"
		// image Ndg en entr�e
		_declspec(dllexport) CImageDouble(const CImageDouble& im); // image en entr�e

		// destructeur
		_declspec(dllexport) ~CImageDouble();

		// pouvoir acc�der � un pixel par image(i)
		_declspec(dllexport) double& operator() (int i) const { 
		return m_pucPixel[i];
		}

		// pouvoir acc�der � un pixel par image(i,j)
		_declspec(dllexport) double& operator() (int i, int j) const { 
		return m_pucPixel[i*m_iLargeur+j];
		}

		// op�rateur copie image par imOut = imIn
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

};

#endif 