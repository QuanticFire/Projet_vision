// ajouter _declspec(dllexport) devant tout public pour permettre à la dll d'exporter ces méthodes 
// pour qu'elles puissent être utilisées par d'autres applications ou programmes

#pragma once

#ifndef _IMAGE_CLASSE_
#define _IMAGE_CLASSE_

#include <string>
#include <vector>

#include "ImageNdg.h"
#include "ImageCouleur.h"

#define EUCLIDEAN(a,b) ((a-b) * (a-b)) 
#define MAX_ITER 100

typedef struct  {
	double  moyenne;
	int     min;
	int     max;
	int     surface;
} SIGNATURE_Ndg;

typedef struct  {
	double  moyenne[3];
	int    surface;
} SIGNATURE_Couleur;

typedef struct {
	float		centreGravite_i;
	float		centreGravite_j;
	int			surface;
	std::string codeFreeman; // au sens V8
	int			premierPt_i; // premier point rencontré sens de parcours avant
	int			premierPt_j;
	int			rectEnglob_Hi; // coins supérieur gauche et inférieur droit
	int			rectEnglob_Hj;
	int			rectEnglob_Bi;
	int			rectEnglob_Bj;
	float		perimetre; // au sens V8
} SIGNATURE_Forme;

typedef struct {
	int     surface;
	double  emergence;
	double  occupation;
} SIGNATURE_Cellule;

// définition classe Image Classe --> images étiquetées pour analyse objets, nuées dynamiques pour analyse régions

class CImageClasse {

	///////////////////////////////////////
	private : 
	///////////////////////////////////////

		int              m_iHauteur;
		int              m_iLargeur;
		std::string      m_sNom;
		long			 m_lNbRegions;

		unsigned long*   m_pucPixel;

	///////////////////////////////////////
	public : 
	///////////////////////////////////////
		
		// constructeurs
		_declspec(dllexport) CImageClasse(); 
		_declspec(dllexport) CImageClasse(int hauteur, int largeur); 
		_declspec(dllexport) CImageClasse(const CImageNdg& im, std::string choix = "V8"); // objets
		_declspec(dllexport) CImageClasse(const CImageClasse& in, std::string choix = "sans", std::string voisinage = "V8"); // re-étiquetage éventuel
		_declspec(dllexport) CImageClasse(const CImageNdg& im, int nbClusters = 2, std::string choix = "aleatoire"); // clustering 
		_declspec(dllexport) CImageClasse(const CImageCouleur& im, int nbClusters = 2, std::string choix = "aleatoire", int plan=0); // plan dans HSV, non dans espace 3D
		_declspec(dllexport) CImageClasse(const CImageNdg& im, const std::vector<int>& germes, double tolerance = 5); // croissange région avec ensemble de germes sous la forme x0,y0,x1,y1,x2,y2 etc
		_declspec(dllexport) ~CImageClasse(); // destructeur

		// sauvegarde au format bmp
		// attention : cast des informations car pertes potentielles
		_declspec(dllexport) void sauvegarde(const std::string& fixe = ""); // sauvegarde data au format BMP avec cast des long en char

		// pouvoir accéder à un pixel par image(i)
		_declspec(dllexport) unsigned long& operator() (int i) const { 
		return m_pucPixel[i];
		}

		// pouvoir accéder à un pixel par image(i,j)
		_declspec(dllexport) unsigned long& operator() (int i, int j) const { 
		return m_pucPixel[i*m_iLargeur+j];
		}

		// opérateur copie image par imOut = imIn
		_declspec(dllexport) CImageClasse& operator=(const CImageClasse& im);

		// get et set 

		_declspec(dllexport) int lireHauteur() const { 
		return m_iHauteur;
		}

		_declspec(dllexport) int lireLargeur() const {
		return m_iLargeur;
		}

		_declspec(dllexport) int lireNbRegions() const {
		return m_lNbRegions;
		}

		_declspec(dllexport) std::string lireNom() const {
		return m_sNom;
		}

		_declspec(dllexport) int lireNbPixels() const {
		return m_iHauteur*m_iLargeur;
		}

		_declspec(dllexport) void ecrireHauteur(int hauteur) {
		m_iHauteur = hauteur;
		}

		_declspec(dllexport) void ecrireLargeur(int largeur) {
		m_iLargeur = largeur;
		}

		_declspec(dllexport) void ecrireNom(std::string nom) {
		m_sNom = nom;
		}

		_declspec(dllexport) void ecrireNbRegions(int nb) {
		m_lNbRegions = nb;
		}

		// signatures pour Image_Ndg et Image_Couleur
		_declspec(dllexport) std::vector<SIGNATURE_Ndg> signatures(const CImageNdg& img, bool enregistrementCSV = false);
		_declspec(dllexport) std::vector<SIGNATURE_Couleur> signatures(const CImageCouleur& img, bool enregistrementCSV = false);

		// affichage
		_declspec(dllexport) CImageCouleur affichage(const std::vector<SIGNATURE_Ndg>& tab, int R=255, int G=0, int B=0);
		_declspec(dllexport) CImageCouleur affichage(const std::vector<SIGNATURE_Couleur>& tab, const std::string& methode = "moyenne");

		// sélection 
		_declspec(dllexport) CImageClasse selection(const std::string& methode = "selection", int classe=0);

		// mutation : conversion depuis Ndg seuillée
		_declspec(dllexport) CImageClasse mutation(const CImageNdg& img);
		_declspec(dllexport) CImageNdg mutation(const std::string& methode = "troncature"); // choix "troncature"

		// filtrage selon critères taille, bords, etc
		_declspec(dllexport) CImageClasse filtrage(const std::string& methode = "taille", int tMin=0, int tMax=100, bool miseAZero = true); // choix "taille" conservation sur [tMin,tMax]
		                                                                                                                                    // choix "maxiLocaux" sur voisinage tMin x tMax
		// signatures forme pour Image_Ndg et Image_Couleur
		_declspec(dllexport) std::vector<SIGNATURE_Forme> signatures(bool enregistrementCSV = false);

		// cellules de Voronoï
		_declspec(dllexport) CImageClasse cellules(); // cellules de Voronoï
		_declspec(dllexport) std::vector<SIGNATURE_Cellule> sigCellules(const CImageNdg& img, bool enregistrementCSV = false);

};

#endif