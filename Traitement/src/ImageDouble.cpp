#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <limits>
#include <stack>
#include <algorithm>

#include "fftw3.h"
#include "ImageDouble.h"

#define PI 3.14159265358979323846

// constructeurs et destructeur
CImageDouble::CImageDouble() {

	this->m_iHauteur  = 0;
	this->m_iLargeur  = 0;
	this->m_sNom      = "vide";
	this->m_vMin      = 0;
	this->m_vMax      = 0;
	this->m_pucPixel  = NULL;
}

CImageDouble::CImageDouble(int hauteur, int largeur, double valeur) {

	this->m_iHauteur = hauteur;
	this->m_iLargeur = largeur;
	this->m_sNom     = "inconnu";
	this->m_vMin     = 0;
	this->m_vMax     = 0;

	this->m_pucPixel = new double[hauteur*largeur];
	for (int i=0;i<hauteur*largeur;i++)
		this->m_pucPixel[i] = valeur;
}

CImageDouble::CImageDouble(const CImageDouble& im) {

	this->m_iHauteur = im.lireHauteur();
	this->m_iLargeur = im.lireLargeur();
	this->m_sNom = im.lireNom();
	this->m_vMax = im.lireMax();
	this->m_vMin = im.lireMin();
	this->m_pucPixel = NULL;

	if (im.m_pucPixel != NULL) {
		this->m_pucPixel = new double[im.lireNbPixels()];
		memcpy(this->m_pucPixel, im.m_pucPixel, im.lireNbPixels()*sizeof(double));
	}
}

CImageDouble::~CImageDouble() {

	if (this->m_pucPixel) {
		delete[] this->m_pucPixel;
		this->m_pucPixel = NULL;
	}
}

CImageDouble::CImageDouble(const CImageNdg& im, const std::string& methode) {

	this->m_iHauteur = im.lireHauteur();
	this->m_iLargeur = im.lireLargeur();

	this->m_pucPixel = new double[im.lireNbPixels()];
	if (methode.compare("normalise") == 0) {
		this->m_sNom = im.lireNom() + "DN";
		this->m_vMin = 0;
		this->m_vMax = 1;
		for (int i = 0; i < this->lireNbPixels(); i++)
			this->operator()(i) = (double)im(i) / 255;
	}
	else
		if (methode.compare("cast") == 0) {
			this->m_sNom = im.lireNom() + "DC";
			this->m_vMin = 255;
			this->m_vMax = 0;
			for (int i = 0; i < this->lireNbPixels(); i++)
			{
				this->operator()(i) = (double)im(i);
				if (this->operator()(i) > this->m_vMax)
					this->m_vMax = this->operator()(i);
				if (this->operator()(i) < this->m_vMin)
					this->m_vMin = this->operator()(i);
			}
		}
		else
			if (methode.compare("integrale1") == 0) {
				this->m_sNom = im.lireNom() + "DI1";

				CImageDouble s(im.lireHauteur(), im.lireLargeur());

				// première ligne
				s(0) = (double)im(0, 0);
				this->operator()(0) = (double)im(0, 0);
				this->ecrireMin((double)im(0, 0));

				for (int j = 1; j < im.lireLargeur(); j++) {
					s(j) = s(j - 1) + (double)im(0, j);
					this->operator()(j) = this->operator()(j - 1) + (double)im(0, j);
				}

				// le reste
				for (int i = 1; i < im.lireHauteur(); i++) {
					s(i, 0) = (double)im(i, 0);
					this->operator()(i, 0) = this->operator()(i - 1, 0) + (double)im(i, 0);
					for (int j = 1; j < im.lireLargeur(); j++) {
						s(i, j) = s(i, j - 1) + (double)im(i, j);
						this->operator()(i, j) = this->operator()(i - 1, j) + s(i, j);
					}
				}

				this->ecrireMax(this->operator()(this->lireNbPixels() - 1));
			}
			else if (methode.compare("integrale2") == 0) {
				this->m_sNom = im.lireNom() + "DI2";
				CImageDouble s(im.lireHauteur(), im.lireLargeur());

				// première ligne
				s(0) = (double)im(0, 0)*(double)im(0, 0);
				this->operator()(0) = (double)im(0, 0)*(double)im(0, 0);
				this->ecrireMin(this->operator()(0));

				for (int j = 1; j < im.lireLargeur(); j++) {
					s(j) = s(j - 1) + (double)im(0, j)*(double)im(0, j);
					this->operator()(j) = this->operator()(j - 1) + (double)im(0, j)*(double)im(0, j);
				}

				// le reste
				for (int i = 1; i < im.lireHauteur(); i++) {
					s(i, 0) = (double)im(i, 0)* (double)im(i, 0);
					this->operator()(i, 0) = this->operator()(i - 1, 0) + (double)im(i, 0)* (double)im(i, 0);
					for (int j = 1; j < im.lireLargeur(); j++) {
						s(i, j) = s(i, j - 1) + (double)im(i, j)*(double)im(i, j);
						this->operator()(i, j) = this->operator()(i - 1, j) + s(i, j);
					}
				}

				this->ecrireMax(this->operator()(this->lireNbPixels() - 1));
			}
}

CImageDouble& CImageDouble::operator=(const CImageDouble& im) {

	if (&im == this)
		return *this;

	this->m_iHauteur = im.lireHauteur();
	this->m_iLargeur = im.lireLargeur();
	this->m_sNom = im.lireNom();
	this->m_vMax = im.lireMax();
	this->m_vMin = im.lireMin();

	if (this->m_pucPixel)
		delete[] this->m_pucPixel;
	this->m_pucPixel = new double[im.lireNbPixels()];

	if (im.m_pucPixel != NULL)
		memcpy(this->m_pucPixel, im.m_pucPixel, im.lireNbPixels()*sizeof(double));

	return *this;
}

// seuillage
CImageDouble CImageDouble::seuillage(double seuilBas, double seuilHaut) {

	// pas de LUT possible, données flottantes
	CImageDouble out(this->lireHauteur(), this->lireLargeur());
	out.m_sNom = this->lireNom() + "S";
	out.m_vMin = 0;
	out.m_vMax = 1;

	for (int i = 0; i < out.lireNbPixels(); i++)
		if ((this->operator()(i) >= seuilBas) && (this->operator()(i) <= seuilHaut))
			out(i) = 1;

	return out;
}


CImageDouble CImageDouble::distance(std::string eltStructurant, double valBord) {
	// distance au fond
	// gestion du bord : 0 ou autre valeur valant max des float

	CImageDouble out(this->lireHauteur(), this->lireLargeur());
	out.ecrireNom(this->lireNom() + "DF");
	out.ecrireMax(DBL_MIN);
	out.ecrireMin(DBL_MAX);

	CImageDouble agrandie(this->lireHauteur() + 2, this->lireLargeur() + 2);
	// gestion des bords ajoutés
	if (valBord != 0) {
		for (int i = 0; i<agrandie.lireHauteur(); i++) {
			agrandie(i, 0) = valBord;
			agrandie(i, agrandie.lireLargeur() - 1) = valBord;
		}
		for (int j = 0; j<agrandie.lireLargeur(); j++) {
			agrandie(0, j) = valBord;
			agrandie(agrandie.lireHauteur() - 1, j) = valBord;
		}
	}

	// coeur image
	for (int i = 0; i<this->lireHauteur(); i++)
		for (int j = 0; j<this->lireLargeur(); j++) {
			agrandie(i + 1, j + 1) = (this->operator()(i, j) > 0) ? (double)INT_MAX-1 : 0;
		}

	if (eltStructurant.compare("V4") == 0) {
		// parcours avant
		for (int i = 1; i<agrandie.lireHauteur() - 1; i++)
			for (int j = 1; j<agrandie.lireLargeur() - 1; j++) {
				double pixel = agrandie(i, j);
				pixel = std::min(pixel, agrandie(i - 1, j) + 1);
				pixel = std::min(pixel, agrandie(i, j - 1) + 1);
				agrandie(i, j) = pixel;
			}
		for (int i = agrandie.lireHauteur() - 2; i >= 1; i--)
			for (int j = agrandie.lireLargeur() - 2; j >= 1; j--) {
				double pixel = agrandie(i, j);
				pixel = std::min(pixel, agrandie(i + 1, j) + 1);
				pixel = std::min(pixel, agrandie(i, j + 1) + 1);
				agrandie(i, j) = pixel;
			}
		// conservation du centre
		for (int i = 1; i<agrandie.lireHauteur() - 1; i++)
			for (int j = 1; j<agrandie.lireLargeur() - 1; j++) {
				out(i - 1, j - 1) = agrandie(i, j);
			}
	}
	else {
		// parcours avant
		for (int i = 1; i<agrandie.lireHauteur() - 1; i++)
			for (int j = 1; j<agrandie.lireLargeur() - 1; j++) {
				double pixel = agrandie(i, j);
				pixel = std::min(pixel, agrandie(i - 1, j) + 1);
				pixel = std::min(pixel, agrandie(i, j - 1) + 1);
				pixel = std::min(pixel, agrandie(i - 1, j - 1) + 1);
				pixel = std::min(pixel, agrandie(i - 1, j + 1) + 1);
				agrandie(i, j) = pixel;
			}
		for (int i = agrandie.lireHauteur() - 2; i >= 1; i--)
			for (int j = agrandie.lireLargeur() - 2; j >= 1; j--) {
				double pixel = agrandie(i, j);
				pixel = std::min(pixel, agrandie(i + 1, j) + 1);
				pixel = std::min(pixel, agrandie(i, j + 1) + 1);
				pixel = std::min(pixel, agrandie(i + 1, j + 1) + 1);
				pixel = std::min(pixel, agrandie(i + 1, j - 1) + 1);
				agrandie(i, j) = pixel;
			}
		// conservation du centre
		for (int i = 1; i<agrandie.lireHauteur() - 1; i++)
			for (int j = 1; j<agrandie.lireLargeur() - 1; j++) {
				out(i - 1, j - 1) = agrandie(i, j);
				if (out(i - 1, j - 1) < out.lireMin())
					out.ecrireMin(out(i - 1, j - 1));
				if (out(i - 1, j - 1) > out.lireMax())
					out.ecrireMax(out(i - 1, j - 1));
			}
	}

	return out;
}

// conversion entre types		 
CImageNdg CImageDouble::toNdg(const std::string& methode) {

	CImageNdg out(this->lireHauteur(), this->lireLargeur());
	out.ecrireBinaire(false);
	out.choixPalette("grise");
	out.ecrireNom(this->lireNom() + "2NDG");

	if (methode.compare("defaut") == 0) {
		for (int i = 0; i < this->lireNbPixels(); i++)
			if (this->operator()(i) < 0)
				out(i) = 0;
			else
				if (this->operator()(i) > 256)
					out(i) = 255;
				else
					out(i) = (unsigned char)this->operator()(i);
	}
	else
		if (methode.compare("expansion") == 0) {
			double a = 255 / (this->lireMax() - this->lireMin());
			double b = -a*this->lireMin();

			for (int i = 0; i < this->lireNbPixels(); i++)
				out(i) = (unsigned char)(a*this->operator()(i)+b);
		}

	return(out);
}

// vecteur gradient
CImageDouble CImageDouble::vecteurGradient(const std::string& axe) {
	CImageDouble out(this->lireHauteur(), this->lireLargeur());

	if (axe.compare("norme") == 0)
	{
		out.ecrireNom(this->lireNom() + "NVG");
		double vMin = sqrt(2) * 255;
		double vMax = 0;

		CImageDouble agrandie(this->lireHauteur() + 2, this->lireLargeur() + 2);

		// gestion des bords et des coins
		int pix;
		agrandie(0, 0) = this->operator()(0, 0);
		agrandie(0, agrandie.lireLargeur() - 1) = this->operator()(0, this->lireLargeur() - 1);
		for (pix = 1; pix < agrandie.lireLargeur() - 1; pix++) {
			agrandie(0, pix) = this->operator()(0, pix - 1);
			agrandie(agrandie.lireHauteur() - 1, pix) = this->operator()(this->lireHauteur() - 1, pix - 1);
		}
		agrandie(agrandie.lireHauteur() - 1, 0) = this->operator()(this->lireHauteur() - 1, 0);
		agrandie(agrandie.lireHauteur() - 1, agrandie.lireLargeur() - 1) = this->operator()(this->lireHauteur() - 1, this->lireLargeur() - 1);
		for (pix = 1; pix < agrandie.lireHauteur() - 1; pix++) {
			agrandie(pix, 0) = this->operator()(pix - 1, 0);
			agrandie(pix, agrandie.lireLargeur() - 1) = this->operator()(pix - 1, this->lireLargeur() - 1);
		}

		// gestion du coeur
		for (int i = 0; i < this->lireHauteur(); i++)
			for (int j = 0; j < this->lireLargeur(); j++) {
				agrandie(i + 1, j + 1) = this->operator()(i, j);
			}

		// passage des noyaux

		for (int i = 1; i < agrandie.lireHauteur() - 1; i++)
			for (int j = 1; j < agrandie.lireLargeur() - 1; j++)
			{
				double vx = agrandie(i + 1, j) - agrandie(i - 1, j);
				double vy = agrandie(i, j + 1) - agrandie(i, j - 1);

				out(i - 1, j - 1) = sqrt((vx*vx) + (vy*vy));
				if (out(i - 1, j - 1) < vMin)
					vMin = out(i - 1, j - 1);
				if (out(i - 1, j - 1) > vMax)
					vMax = out(i - 1, j - 1);
			}

		out.ecrireMin(vMin);
		out.ecrireMax(vMax);
	}
	else
		if (axe.compare("angle") == 0)
		{
			out.ecrireNom(this->lireNom() + "AVG");

			CImageDouble agrandie(this->lireHauteur() + 2, this->lireLargeur() + 2);

			// gestion des bords et des coins
			int pix;
			agrandie(0, 0) = this->operator()(0, 0);
			agrandie(0, agrandie.lireLargeur() - 1) = this->operator()(0, this->lireLargeur() - 1);
			for (pix = 1; pix < agrandie.lireLargeur() - 1; pix++) {
				agrandie(0, pix) = this->operator()(0, pix - 1);
				agrandie(agrandie.lireHauteur() - 1, pix) = this->operator()(this->lireHauteur() - 1, pix - 1);
			}
			agrandie(agrandie.lireHauteur() - 1, 0) = this->operator()(this->lireHauteur() - 1, 0);
			agrandie(agrandie.lireHauteur() - 1, agrandie.lireLargeur() - 1) = this->operator()(this->lireHauteur() - 1, this->lireLargeur() - 1);
			for (pix = 1; pix < agrandie.lireHauteur() - 1; pix++) {
				agrandie(pix, 0) = this->operator()(pix - 1, 0);
				agrandie(pix, agrandie.lireLargeur() - 1) = this->operator()(pix - 1, this->lireLargeur() - 1);
			}

			// gestion du coeur
			for (int i = 0; i < this->lireHauteur(); i++)
				for (int j = 0; j < this->lireLargeur(); j++) {
					agrandie(i + 1, j + 1) = this->operator()(i, j);
				}

			// passage des noyaux

			for (int i = 1; i < agrandie.lireHauteur() - 1; i++)
				for (int j = 1; j < agrandie.lireLargeur() - 1; j++)
				{
					double vx = agrandie(i + 1, j) - agrandie(i - 1, j);
					double vy = agrandie(i, j + 1) - agrandie(i, j - 1);
					out(i - 1, j - 1) = atan2(vy, vx) * 180 / PI;
				}

			// fixer intervall -PI.. PI soit -180 .. 180 en degres
			out.ecrireMin(-180); 
			out.ecrireMax(180);
		}

	return out;
}

// filtrage : moyen ou gaussien (approches par vecteur et transposée plus rapide)
CImageDouble CImageDouble::filtrage(const std::string& methode, int N, double sigma) {

	CImageDouble out(this->lireHauteur(), this->lireLargeur());
	out.m_vMax = DBL_MIN;
	out.m_vMin = DBL_MAX;

	if (methode.compare("moyen") == 0) {
		out.m_sNom = this->lireNom() + "FMo";
		int nbBords = N / 2;

		CImageDouble agrandie(this->lireHauteur() + nbBords * 2, this->lireLargeur() + nbBords * 2);

		// gestion du coeur
		for (int i = 0; i < this->lireHauteur(); i++)
			for (int j = 0; j < this->lireLargeur(); j++) {
				agrandie(i + nbBords, j + nbBords) = this->operator()(i, j);
			}

		// gestion des bords
		for (int pix = 0; pix < agrandie.lireLargeur(); pix++) {
			for (int t = nbBords - 1; t >= 0; t--)
				agrandie(t, pix) = agrandie(nbBords, pix);
			for (int t = agrandie.lireHauteur() - 1; t >= agrandie.lireHauteur() - 1 - nbBords; t--)
				agrandie(t, pix) = agrandie(agrandie.lireHauteur() - 1 - nbBords, pix);
		}
		for (int pix = 0; pix < agrandie.lireHauteur(); pix++) {
			for (int t = nbBords - 1; t >= 0; t--)
				agrandie(pix, t) = agrandie(pix, nbBords);
			for (int t = agrandie.lireLargeur() - 1; t >= agrandie.lireLargeur() - 1 - nbBords; t--)
				agrandie(pix, t) = agrandie(pix, agrandie.lireLargeur() - 1 - nbBords);
		}

		CImageDouble agrandie2 = agrandie;

		// colonnes
		for (int i = nbBords; i < agrandie.lireHauteur() - nbBords; i++)
			for (int j = nbBords; j < agrandie.lireLargeur() - nbBords; j++) {
				double somme = 0;
				double moy = 0;

				for (int k = -nbBords; k <= nbBords; k++) {
					moy += (double)agrandie(i - k, j);
					somme += (double)1;
				}
				agrandie2(i, j) = moy / somme;
			}
		// lignes
		for (int i = nbBords; i < agrandie.lireHauteur() - nbBords; i++)
			for (int j = nbBords; j < agrandie.lireLargeur() - nbBords; j++) {
				double somme = 0;
				double moy = 0;

				for (int l = -nbBords; l <= nbBords; l++) {
					moy += (double)agrandie2(i, j - l);
					somme += (double)1;
				}
				agrandie(i, j) = (moy / somme);
			}
		// image out
		for (int i = nbBords; i < agrandie.lireHauteur() - nbBords; i++)
			for (int j = nbBords; j < agrandie.lireLargeur() - nbBords; j++)
			{
				out(i - nbBords, j - nbBords) = agrandie(i, j);
				if (out(i - nbBords, j - nbBords) < out.lireMin())
					out.ecrireMin(out(i - nbBords, j - nbBords));
				if (out(i - nbBords, j - nbBords) > out.lireMax())
					out.ecrireMax(out(i - nbBords, j - nbBords));
			}
	}
	else
		if (methode.compare("gaussien") == 0)
		{
			out.m_sNom = this->lireNom() + "FGa";
			// définition du noyau
			double noyau[50]; // taille maxi pour optimisation 

			double somme = 0; // normalisation
			for (int i = 0; i < N; i++)
			{
				noyau[i] = exp(-((i - N / 2)*(i - N / 2)) / (2 * sigma*sigma));
				somme += noyau[i];
			}

			// filtrage
			int nbBords = N / 2;

			CImageDouble agrandie(this->lireHauteur() + nbBords * 2, this->lireLargeur() + nbBords * 2);

			// gestion du coeur
			for (int i = 0; i < this->lireHauteur(); i++)
				for (int j = 0; j < this->lireLargeur(); j++) {
					agrandie(i + nbBords, j + nbBords) = this->operator()(i, j);
				}

			// gestion des bords
			for (int pix = 0; pix < agrandie.lireLargeur(); pix++) {
				for (int t = nbBords - 1; t >= 0; t--)
					agrandie(t, pix) = agrandie(nbBords, pix);
				for (int t = agrandie.lireHauteur() - 1; t >= agrandie.lireHauteur() - 1 - nbBords; t--)
					agrandie(t, pix) = agrandie(agrandie.lireHauteur() - 1 - nbBords, pix);
			}
			for (int pix = 0; pix < agrandie.lireHauteur(); pix++) {
				for (int t = nbBords - 1; t >= 0; t--)
					agrandie(pix, t) = agrandie(pix, nbBords);
				for (int t = agrandie.lireLargeur() - 1; t >= agrandie.lireLargeur() - 1 - nbBords; t--)
					agrandie(pix, t) = agrandie(pix, agrandie.lireLargeur() - 1 - nbBords);
			}

			CImageDouble agrandie2 = agrandie;

			// colonnes
			for (int i = nbBords; i < agrandie.lireHauteur() - nbBords; i++)
				for (int j = nbBords; j < agrandie.lireLargeur() - nbBords; j++) {
					double somme = 0;
					double moy = 0;

					for (int k = -nbBords; k <= nbBords; k++) {
						moy += (double)agrandie(i - k, j)*noyau[k + nbBords];
						somme += noyau[k + nbBords];
					}
					agrandie2(i, j) = moy / somme;
				}
			// lignes
			for (int i = nbBords; i < agrandie.lireHauteur() - nbBords; i++)
				for (int j = nbBords; j < agrandie.lireLargeur() - nbBords; j++) {
					double somme = 0;
					double moy = 0;

					for (int l = -nbBords; l <= nbBords; l++) {
						moy += (double)agrandie2(i, j - l)*noyau[l + nbBords];
						somme += noyau[l + nbBords];
					}
					agrandie(i, j) = (moy / somme);
				}
			// image out
			for (int i = nbBords; i < agrandie.lireHauteur() - nbBords; i++)
				for (int j = nbBords; j < agrandie.lireLargeur() - nbBords; j++)
				{
					out(i - nbBords, j - nbBords) = agrandie(i, j);
					if (out(i - nbBords, j - nbBords) < out.lireMin())
						out.ecrireMin(out(i - nbBords, j - nbBords));
					if (out(i - nbBords, j - nbBords) > out.lireMax())
						out.ecrireMax(out(i - nbBords, j - nbBords));
				}
		}

	return out;
}

std::vector<CImageDouble> CImageDouble::pyramide(int hauteur, int tailleFiltre, double sigma)
{
	std::vector<CImageDouble> burt;

	// propagation des min,max du niveau initial aux autres niveaux, sinon va modifier la dynamique des niveaux
	burt.resize(hauteur);
	burt.at(0) = *this;
	burt.at(0).ecrireNom(this->lireNom() + std::to_string(0));

	for (int niv = 1; niv < hauteur; niv++)
	{
		// filtrage gaussien qui améliore les résultats avant sous-échantillonage
		CImageDouble niveau = burt.at(niv - 1).filtrage("gaussien", 5, 1);
		CImageDouble inter(niveau.lireHauteur() / 2, niveau.lireLargeur() / 2);

		inter.ecrireMax(this->lireMax()); // pour ne pas changer la dynamique entre niveaux
		inter.ecrireMin(this->lireMin());
		inter.ecrireNom(this->lireNom() + std::to_string(niv));

		for (int i = 0; i < inter.lireHauteur(); i++)
			for (int j = 0; j < inter.lireLargeur(); j++)
				inter(i, j) = (niveau(2 * i, 2 * j) + niveau(2 * i + 1, 2 * j) + niveau(2 * i, 2 * j + 1) + niveau(2 * i + 1, 2 * j + 1)) / 4;

		burt.at(niv) = inter;
	}

	return burt;
}

// transformée de Hough

CImageDouble CImageDouble::maxiLocaux(int N, int M) const {

	CImageDouble out(this->lireHauteur(), this->lireLargeur());

	int ns2 = N / 2;
	int ms2 = M / 2;

	out.ecrireNom(this->lireNom() + "ML");
	for (int i = 0; i<this->lireHauteur(); i++)
		for (int j = 0; j<this->lireLargeur(); j++)
			if (this->operator()(i, j) > 0) {	// test si le pixel existe i-taille/2
				int dk = std::max(0, i - ns2);
				int fk = std::min(i + ns2, this->lireHauteur() - 1);
				int dl = std::max(0, j - ms2);
				int fl = std::min(j + ms2, this->lireLargeur() - 1);

				double maxVal = this->operator()(i, j);
				bool flag = true;
				int k = dk;
				while ((k <= fk) && (flag == true)) {
					int l = dl;
					while ((l <= fl) && (flag == true)) {
						if (this->operator()(k, l) > maxVal)
							flag = false;
						l++;
					}
					k++;
				}
				if (flag == true)
					out(i, j) = 1;
			}
	out.m_vMax = 1;

	return out;
}

CImageDouble CImageDouble::planHough() {

	double hough_h = std::max(this->lireHauteur() / 2, this->lireLargeur() / 2)*sqrt(2.0);

	CImageDouble H((int)(hough_h * 2), 180);
	H.ecrireMin(0);
	H.ecrireMax(0);

	double cx = this->lireLargeur() / 2;
	double cy = this->lireHauteur() / 2;

	//HOUGH transformation  

	for (int y = 0; y < this->lireHauteur(); y++)
	{
		for (int x = 0; x < this->lireLargeur(); x++)
		{
			if (this->operator()(y, x) > 0)
				for (int t = 0; t < 180; t++)
				{
					double r = (((double)x - cx) * cos((double)t * (PI / 180)) + (((double)y - cy) * sin((double)t * (PI / 180))));
					H(int(hough_h + r), t) += 1;
				}
		}
	}

	for (int p = 0; p < H.lireNbPixels(); p++)
		if (H(p) > H.lireMax())
			H.ecrireMax(H(p));

	return H;
}

CImageDouble CImageDouble::extractionLignes(int N, int M, double dimLigne, bool affichage) {

	// extraction des maxi locaux sur voisinage NxM
	CImageDouble mL = this->maxiLocaux(N, M);
	double hough_h = this->lireHauteur() / 2;

	// extraction des lignes avec maxi local + seuil 

	CImageDouble lignes(this->lireHauteur(), this->lireLargeur());
	lignes.ecrireNom("lignes");

	int nbLignes = 1;
	std::stack<int> angles;
	std::stack<double> rho;

	for (int r = 0; r < this->lireHauteur(); r++)
		for (int a = 0; a < this->lireLargeur(); a++)
			if ((mL(r, a) >0) && (this->operator()(r, a) >= dimLigne))
			{
				lignes(r, a) = nbLignes++;
				angles.push(a);
				rho.push(r - hough_h);
			}
	if (affichage)
	{
		std::cout << nbLignes - 1 << " lignes avec les infos suivants :" << std::endl;
		while (!angles.empty())
		{
			std::cout << "(" << angles.top() << "," << rho.top() << ")";
			angles.pop();
			rho.pop();

			if (!angles.empty())
				std::cout << " ; ";
		}
		std::cout << std::endl;
	}

	lignes.m_vMin = 0;
	lignes.m_vMax = nbLignes - 1;

	return lignes;
}

CImageNdg CImageDouble::houghInverse(const CImageNdg& img) {

	CImageNdg HI(img.lireHauteur(), img.lireLargeur(), 0);
	HI.ecrireNom(img.lireNom() + "HI");
	HI.choixPalette(img.lirePalette());

	double hough_h = std::max(img.lireHauteur() / 2, img.lireLargeur() / 2)*sqrt(2.0);

	double cx = img.lireLargeur() / 2;
	double cy = img.lireHauteur() / 2;

	for (int y = 0; y < img.lireHauteur(); y++)
	{
		for (int x = 0; x < img.lireLargeur(); x++)
		{
			if (img(y, x) > 0)
			{
				for (int t = 0; t < 180; t++)
				{
					double r = (((double)x - cx) * cos((double)t * (PI / 180)) + (((double)y - cy) * sin((double)t * (PI / 180))));
					if (this->operator()(int(hough_h + r), t) > 0)
						HI(y, x) = (int)(this->operator()(int(hough_h + r), t)) % 255;
				}
			}
		}
	}

	return(HI);
}

typedef struct pics {
	int numero;
	double angles;
	double rhos;
	double taille;
} PICS;

static bool myTri(PICS p1, PICS p2) {
	return (p1.taille > p2.taille);
}


CImageNdg CImageDouble::houghExtractionLignes(const CImageNdg &img, const std::string& methode,  int N, int M, int dim, int nombre, bool enregistrementCSV) {
	// HOUGH transform
	double hough_h = std::max(img.lireHauteur() / 2, img.lireLargeur() / 2)*sqrt(2.0);
	int DIAG = (int)(hough_h * 2);
	CImageDouble planHough(DIAG, 180);

	planHough.ecrireMin(0);
	planHough.ecrireMax(0);

	double cx = img.lireHauteur() / 2;
	double cy = img.lireLargeur() / 2;

	for (int x = 0; x < img.lireHauteur(); x++)
	{
		for (int y = 0; y < img.lireLargeur(); y++)
		{
			if (img(x, y) > 0)
			{
				for (int t = 0; t <= 179; t++)
				{
					double r = (((double)x - cx) * cos((double)t * (PI / 180)) + (((double)y - cy) * sin((double)t * (PI / 180))));
					planHough(int(hough_h + r), t) += 1;
				}
			}
		}

	}

	// filtrage gaussien
	//planHough = planHough.filtrage("gaussien", 5, 1); // optionnel 

	for (int p = 0; p < planHough.lireNbPixels(); p++)
		if (planHough(p) > planHough.lireMax())
			planHough.ecrireMax(planHough(p));
	planHough.toNdg("expansion").sauvegarde("planHough");

	// extraction maxi locaux
	CImageDouble mL = planHough.maxiLocaux(N, M);

	// Hough inverse
	CImageNdg HI(img.lireHauteur(), img.lireLargeur(), 0);
	HI.ecrireNom(img.lireNom() + "HI");
	HI.choixPalette("binaire");

	if (methode.compare("longueur") == 0) {
		// extraction pics
		int nbLignes = 1;

		std::stack<PICS> pics;

		for (int r = 0; r < DIAG; r++)
			for (int a = 0; a <= 179; a++)
				if ((mL(r, a) > 0) && (planHough(r, a) >= dim))
				{
					PICS pic;
					pic.numero = nbLignes;
					pic.angles = a;
					pic.rhos = r - hough_h;
					pic.taille = planHough(r, a);
					pics.push(pic);
					nbLignes += 1;
				}

		// dépilement pics

		if (enregistrementCSV) {
			std::string fichier = "res/" + img.lireNom() + "_Pics.csv";
			std::ofstream f(fichier.c_str());

			if (!f.is_open())
				std::cout << "Impossible d'ouvrir le fichier en ecriture !" << std::endl;
			else {
				f << "Numero; Distance; Angle; Taille" << std::endl;
				while (!pics.empty())
				{
					PICS pic = pics.top();
					int label = pic.numero;
					double angle = pic.angles;
					double rho = pic.rhos;
					double tai = pic.taille;

					f << label << " ; " << rho << " ; " << angle << " ; " << tai << std::endl;
					if (angle != 0)
					{
						for (int i = 0; i < img.lireHauteur(); i++)
						{
							double x = i - cx;
							int y = (int)((rho - x*cos(angle * (PI / 180))) / sin(angle * (PI / 180)));
							int j = (int)(y + cy);
							if ((j >= 0) && (j < img.lireLargeur()))
								HI(i, j) = label;
						}
						for (int j = 0; j < img.lireLargeur(); j++)
						{
							double y = j - cy;
							int x = (int)((rho - y*sin(angle * (PI / 180))) / cos(angle * (PI / 180)));
							int i = (int)(x + cx);
							if ((i >= 0) && (i < img.lireHauteur()))
								HI(i, j) = label;
						}
					}
					else
						if (angle != 90)
						{
							for (int j = 0; j < img.lireLargeur(); j++) {
								double y = j - cy;
								int x = (int)((rho - y*sin(angle*PI / 180)) / cos(angle*PI / 180));
								int i = (int)(x + cx);
								if ((i >= 0) && (i < img.lireHauteur()))
									HI(i, j) = label;
							}
							for (int i = 0; i < img.lireHauteur(); i++)
							{
								double x = i - cx;
								int y = (int)((rho - x*cos(angle * (PI / 180))) / sin(angle * (PI / 180)));
								int j = (int)(y + cy);
								if ((j >= 0) && (j < img.lireLargeur()))
									HI(i, j) = label;
							}
						}
					pics.pop();
				}
				f.close();
			}
		}
		else {
			while (!pics.empty())
			{
				PICS pic = pics.top();
				int label = pic.numero;
				double angle = pic.angles;
				double rho = pic.rhos;
				std::cout << label << " " << angle << " " << rho << std::endl;
				if (angle != 0)
				{
					for (int i = 0; i < img.lireHauteur(); i++)
					{
						double x = i - cx;
						int y = (int)((rho - x*cos(angle * (PI / 180))) / sin(angle * (PI / 180)));
						int j = (int)(y + cy);
						if ((j >= 0) && (j < img.lireLargeur()))
							HI(i, j) = label;
					}
					for (int j = 0; j < img.lireLargeur(); j++)
					{
						double y = j - cy;
						int x = (int)((rho - y*sin(angle * (PI / 180))) / cos(angle * (PI / 180)));
						int i = (int)(x + cx);
						if ((i >= 0) && (i < img.lireHauteur()))
							HI(i, j) = label;
					}
				}
				else
					if (angle != 90)
					{
						for (int j = 0; j < img.lireLargeur(); j++) {
							double y = j - cy;
							int x = (int)((rho - y*sin(angle*PI / 180)) / cos(angle*PI / 180));
							int i = (int)(x + cx);
							if ((i >= 0) && (i < img.lireHauteur()))
								HI(i, j) = label;
						}
						for (int i = 0; i < img.lireHauteur(); i++)
						{
							double x = i - cx;
							int y = (int)((rho - x*cos(angle * (PI / 180))) / sin(angle * (PI / 180)));
							int j = (int)(y + cy);
							if ((j >= 0) && (j < img.lireLargeur()))
								HI(i, j) = label;
						}
					}
				pics.pop();
			}
		}
	}
	else
		if (methode.compare("nombre") == 0) {
			// extraction pics
			int nbLignes = 1;

			std::vector<PICS> pics;

			for (int r = 0; r < DIAG; r++)
				for (int a = 0; a <= 179; a++)
					if ((mL(r, a) > 0) && (planHough(r, a) >= dim)) // base minimale pour comptabiliser une droite
					{
						PICS pic;
						pic.numero = nbLignes;
						pic.angles = a;
						pic.rhos = r - hough_h;
						pic.taille = planHough(r, a);
						pics.push_back(pic);
						nbLignes += 1;
					}

			int nbPics = std::min(nbLignes - 1, nombre); // si pas suffisamment de pics extrait
														 // dépilement pics

			sort(pics.begin(), pics.end(), myTri);

			if (enregistrementCSV) {
				std::string fichier = "res/" + img.lireNom() + "_Pics.csv";
				std::ofstream f(fichier.c_str());

				if (!f.is_open())
					std::cout << "Impossible d'ouvrir le fichier en ecriture !" << std::endl;
				else {
					f << "Numero; Distance; Angle; Taille" << std::endl;
					for (int p = 0; p < nbPics; p++)
					{
						PICS pic = pics.at(p);
						int label = pic.numero;
						double angle = pic.angles;
						double rho = pic.rhos;
						double tai = pic.taille;

						f << label << " ; " << rho << " ; " << angle << " ; " << tai << std::endl;

						if (angle != 0)
						{
							for (int i = 0; i < img.lireHauteur(); i++)
							{
								double x = i - cx;
								int y = (int)((rho - x*cos(angle * (PI / 180))) / sin(angle * (PI / 180)));
								int j = (int)(y + cy);
								if ((j >= 0) && (j < img.lireLargeur()))
									HI(i, j) = label;
							}
							for (int j = 0; j < img.lireLargeur(); j++)
							{
								double y = j - cy;
								int x = (int)((rho - y*sin(angle * (PI / 180))) / cos(angle * (PI / 180)));
								int i = (int)(x + cx);
								if ((i >= 0) && (i < img.lireHauteur()))
									HI(i, j) = label;
							}
						}
						else
							if (angle != 90)
							{
								for (int j = 0; j < img.lireLargeur(); j++) {
									double y = j - cy;
									int x = (int)((rho - y*sin(angle*PI / 180)) / cos(angle*PI / 180));
									int i = (int)(x + cx);
									if ((i >= 0) && (i < img.lireHauteur()))
										HI(i, j) = label;
								}
								for (int i = 0; i < img.lireHauteur(); i++)
								{
									double x = i - cx;
									int y = (int)((rho - x*cos(angle * (PI / 180))) / sin(angle * (PI / 180)));
									int j = (int)(y + cy);
									if ((j >= 0) && (j < img.lireLargeur()))
										HI(i, j) = label;
								}
							}
					}
					f.close();
				}
			}
			else {
				for (int p = 0; p < nbPics; p++)
				{
					PICS pic = pics.at(p);
					int label = pic.numero;
					double angle = pic.angles;
					double rho = pic.rhos;
					double tai = pic.taille;

					std::cout << label << " " << angle << " " << rho << " -> " << tai << std::endl;
					if (angle != 0)
					{
						for (int i = 0; i < img.lireHauteur(); i++)
						{
							double x = i - cx;
							int y = (int)((rho - x*cos(angle * (PI / 180))) / sin(angle * (PI / 180)));
							int j = (int)(y + cy);
							if ((j >= 0) && (j < img.lireLargeur()))
								HI(i, j) = label;
						}
						for (int j = 0; j < img.lireLargeur(); j++)
						{
							double y = j - cy;
							int x = (int)((rho - y*sin(angle * (PI / 180))) / cos(angle * (PI / 180)));
							int i = (int)(x + cx);
							if ((i >= 0) && (i < img.lireHauteur()))
								HI(i, j) = label;
						}
					}
					else
						if (angle != 90)
						{
							for (int j = 0; j < img.lireLargeur(); j++) {
								double y = j - cy;
								int x = (int)((rho - y*sin(angle*PI / 180)) / cos(angle*PI / 180));
								int i = (int)(x + cx);
								if ((i >= 0) && (i < img.lireHauteur()))
									HI(i, j) = label;
							}
							for (int i = 0; i < img.lireHauteur(); i++)
							{
								double x = i - cx;
								int y = (int)((rho - x*cos(angle * (PI / 180))) / sin(angle * (PI / 180)));
								int j = (int)(y + cy);
								if ((j >= 0) && (j < img.lireLargeur()))
									HI(i, j) = label;
							}
						}
				}
			}
		}

	return(HI);

}


double CImageDouble::moyenne() const
{
	double somme = 0;
	for (int i = 0; i < this->lireNbPixels(); i++)
		somme += this->operator()(i);
	somme /= this->lireNbPixels();
	return somme;
}

CImageDouble CImageDouble::conv(const CImageDouble& kernel)
{
	CImageDouble out(this->lireHauteur() + kernel.lireHauteur() - 1, this->lireLargeur() + kernel.lireLargeur() - 1);
	for (int j = 0; j < out.lireHauteur(); j++)
	{
		for (int k = 0; k < out.lireLargeur(); k++)
		{
			double sumjk = 0;
			int lp = std::max(0, j + 1 - kernel.lireHauteur());
			int hp = std::min(this->lireHauteur(), j + 1);
			int lq = std::max(0, k + 1 - kernel.lireLargeur());
			int hq = std::min(this->lireLargeur(), k + 1);
			for (int p = lp; p < hp; p++)
				for (int q = lq; q < hq; q++)
					sumjk += kernel.m_pucPixel[(j - p)*kernel.m_iLargeur + (k - q)] * m_pucPixel[p*m_iLargeur + q];
			out(j, k) = sumjk;
		}
	}
	return out;
}

// convolution avec fft, n'est important que pour avoir un temps de traitement plus raisonnable
CImageDouble CImageDouble::convfft(const CImageDouble& kernel)
{
	//padding supplémentaire pour eviter les effets de bord
	int hauteur = (this->lireHauteur() + kernel.lireHauteur() + 2) / 2;
	int largeur = (this->lireLargeur() + kernel.lireLargeur() + 2) / 2;
	CImageDouble out(this->lireHauteur() + kernel.lireHauteur() - 1, this->lireLargeur() + kernel.lireLargeur() - 1);
	double* outdata = new double[4 * hauteur*largeur]; //approximation en taille de la convolution réelle

													   //padding sur les images pour éviter le cyclique
	CImageDouble thispad(hauteur * 2, largeur * 2);
	CImageDouble kernelpad(hauteur * 2, largeur * 2);

	int	offsetthisi = (hauteur * 2 - this->lireHauteur()) / 2;
	int offsetthisj = (largeur * 2 - this->lireLargeur()) / 2;
	int offsetkerneli = (hauteur * 2 - kernel.lireHauteur()) / 2;
	int offsetkernelj = (largeur * 2 - kernel.lireLargeur()) / 2;

	for (int i = 0; i < this->lireHauteur(); i++)
		for (int j = 0; j < this->lireLargeur(); j++)
			thispad(offsetthisi + i, offsetthisj + j) = this->operator()(i, j);
	for (int i = 0; i < kernel.lireHauteur(); i++)
		for (int j = 0; j < kernel.lireLargeur(); j++)
			kernelpad(offsetkerneli + i, offsetkernelj + j) = kernel(i, j);

	// DFTs avec fftw
	fftw_complex *thisdft, *kerneldft, *outdft;
	fftw_plan thisplan, kernelplan, outplan;
	// allocation de cette façon car passage réel complexe donne une dimension h*(l/2+1)
	thisdft = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * 2 * hauteur*(largeur + 1));
	kerneldft = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * 2 * hauteur*(largeur + 1));
	outdft = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * 2 * hauteur*(largeur + 1));

	CImageDouble test(this->lireHauteur() + kernel.lireHauteur() - 1, this->lireLargeur() + kernel.lireLargeur() - 1);

	// une estimantion suffit généralement.
	thisplan = fftw_plan_dft_r2c_2d(2 * hauteur, 2 * largeur, thispad.m_pucPixel, thisdft, FFTW_ESTIMATE);
	kernelplan = fftw_plan_dft_r2c_2d(2 * hauteur, 2 * largeur, kernelpad.m_pucPixel, kerneldft, FFTW_ESTIMATE);

	fftw_execute(thisplan);
	fftw_execute(kernelplan);

	//convolution au sens fft
	for (int n = 0; n < 2 * hauteur*(largeur + 1); n++)
	{
		outdft[n][0] = thisdft[n][0] * kerneldft[n][0] - thisdft[n][1] * kerneldft[n][1];
		outdft[n][1] = thisdft[n][0] * kerneldft[n][1] + thisdft[n][1] * kerneldft[n][0];
	}

	//transformée inverse
	outplan = fftw_plan_dft_c2r_2d(2 * hauteur, 2 * largeur, outdft, outdata, FFTW_ESTIMATE);
	fftw_execute(outplan);

	int x, y;
	int paddingx = (2 * hauteur - out.lireHauteur());
	int paddingy = (2 * largeur - out.lireLargeur());

	//normalisation et suppression du padding
	for (int i = 0; i < out.lireHauteur(); i++)
		for (int j = 0; j < out.lireLargeur(); j++)
		{
			x = i;
			y = j;
			if (i <= hauteur - paddingx && j < largeur - paddingy) { x = i; y = j; }
			if (i > hauteur - paddingx && j < largeur - paddingy) { x = i + paddingx; y = j; }
			if (i <= hauteur - paddingx && j >= largeur - paddingy) { x = i; y = j + paddingy; }
			if (i > hauteur - paddingx && j >= largeur - paddingy) { x = i + paddingx; y = j + paddingy; }
			test(i, j) = outdata[y + 2 * largeur*x] / (4 * hauteur*largeur);
		}

	// centrage
	for (int i = 0; i < out.lireHauteur(); i++)
		for (int j = 0; j < out.lireLargeur(); j++)
		{
			x = i;
			y = j;
			if (i<out.m_iHauteur / 2 && j<out.m_iLargeur / 2) { x = i + out.m_iHauteur / 2; y = j + out.m_iLargeur / 2; }
			if (i >= out.m_iHauteur / 2 && j<out.m_iLargeur / 2) { x = i - out.m_iHauteur / 2; y = j + out.m_iLargeur / 2; }
			if (i<out.m_iHauteur / 2 && j >= out.m_iLargeur / 2) { x = i + out.m_iHauteur / 2; y = j - out.m_iLargeur / 2; }
			if (i >= out.m_iHauteur / 2 && j >= out.m_iLargeur / 2) { x = i - out.m_iHauteur / 2; y = j - out.m_iLargeur / 2; }
			out(i, j) = test(x, y);
		}

	//libération
	fftw_destroy_plan(thisplan);
	fftw_destroy_plan(kernelplan);
	fftw_destroy_plan(outplan);
	fftw_free(thisdft);
	fftw_free(kerneldft);
	fftw_free(outdft);
	free(outdata);


	return out;
}

CImageDouble CImageDouble::NormCorr(const CImageDouble & scene)
{
	CImageDouble out;
	out.m_vMax = -DBL_MAX;
	out.m_vMin = DBL_MAX;

	if ((scene.lireHauteur() < this->lireHauteur()) && (scene.lireLargeur() < this->lireLargeur()))
		out.ecrireNom("patternPlusGrandQueScène!");
	else
	{
		//allocations
		out.ecrireNom(this->lireNom() + "Match" + scene.lireNom());
		CImageDouble a(*this);
		CImageDouble a1(this->lireHauteur(), this->lireLargeur(), 1.0); a1.ecrireNom("uns");
		CImageDouble ar(this->lireHauteur(), this->lireLargeur()); ar.ecrireNom(this->lireNom() + "NormRenverse");
		CImageDouble b(scene);
		CImageDouble bconv, bconv2;
		CImageDouble b2(scene.lireHauteur(), scene.lireLargeur());
		double moythis, moyscene, NbpixThis = this->lireNbPixels(), sum2a = 0;
		//normalisation et somme carrée des pixels du pattern
		moythis = this->moyenne();
		moyscene = scene.moyenne();
		for (int i = 0; i < this->lireNbPixels(); i++)
		{
			a(i) -= moythis;
			ar(i) = this->operator()(this->lireNbPixels() - i - 1) - moythis;
			sum2a += a(i)*a(i);
		}
		//normalisation et image carrée de la scène
		for (int i = 0; i < scene.lireNbPixels(); i++)
		{
			b(i) -= moyscene;
			b2(i) = b(i)*b(i);
		}
		//convolutions et calcul corrélation
		out = b.convfft(ar);
		bconv = b.convfft(a1);
		bconv2 = b2.convfft(a1);

		for (int i = 0; i < bconv.lireNbPixels(); i++)
			bconv(i) = std::max(bconv2(i) - bconv(i)*bconv(i) / NbpixThis, 0.0); //éviter des valeurs négatives à cause de la soustraction

		for (int i = 0; i < out.lireNbPixels(); i++)
		{
			out(i) = out(i) / (sqrt(bconv(i)*sum2a));
			if (out(i) < out.lireMin())		//changement de la dynamique
				out.ecrireMin(out(i));
			if (out(i) > out.lireMax())
				out.ecrireMax(out(i));
		}
	}
	return out;
}

#undef PICS
