#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <cmath>
#include <vector>
#include <limits>
#include <stack>

#include "ImageDouble.h"

#define PI 3.14159265

// constructeurs et destructeur
CImageDouble::CImageDouble() {

	this->m_iHauteur  = 0;
	this->m_iLargeur  = 0;
	this->m_sNom      = "vide";
	this->m_vMin      = 0;
	this->m_vMax      = 0;
	this->m_pucPixel  = NULL;
}

CImageDouble::CImageDouble(int hauteur, int largeur) {

	this->m_iHauteur = hauteur;
	this->m_iLargeur = largeur;
	this->m_sNom     = "inconnu";
	this->m_vMin     = 0;
	this->m_vMax     = 0;

	this->m_pucPixel = new double[hauteur*largeur];
	for (int i=0;i<hauteur*largeur;i++)
		this->m_pucPixel[i] = 0;
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
	out.ecrireMax(FLT_MIN);
	out.ecrireMin(FLT_MAX);

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
				pixel = min(pixel, agrandie(i - 1, j) + 1);
				pixel = min(pixel, agrandie(i, j - 1) + 1);
				agrandie(i, j) = pixel;
			}
		for (int i = agrandie.lireHauteur() - 2; i >= 1; i--)
			for (int j = agrandie.lireLargeur() - 2; j >= 1; j--) {
				double pixel = agrandie(i, j);
				pixel = min(pixel, agrandie(i + 1, j) + 1);
				pixel = min(pixel, agrandie(i, j + 1) + 1);
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
				pixel = min(pixel, agrandie(i - 1, j) + 1);
				pixel = min(pixel, agrandie(i, j - 1) + 1);
				pixel = min(pixel, agrandie(i - 1, j - 1) + 1);
				pixel = min(pixel, agrandie(i - 1, j + 1) + 1);
				agrandie(i, j) = pixel;
			}
		for (int i = agrandie.lireHauteur() - 2; i >= 1; i--)
			for (int j = agrandie.lireLargeur() - 2; j >= 1; j--) {
				double pixel = agrandie(i, j);
				pixel = min(pixel, agrandie(i + 1, j) + 1);
				pixel = min(pixel, agrandie(i, j + 1) + 1);
				pixel = min(pixel, agrandie(i + 1, j + 1) + 1);
				pixel = min(pixel, agrandie(i + 1, j - 1) + 1);
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

CImageDouble CImageDouble::maxiLocaux(int N, int M) const {

	CImageDouble out(this->lireHauteur(), this->lireLargeur());

	int ns2 = N / 2;
	int ms2 = M / 2;

	out.ecrireNom(this->lireNom() + "ML");
	for (int i = 0; i<this->lireHauteur(); i++)
		for (int j = 0; j<this->lireLargeur(); j++)
			if (this->operator()(i, j) > 0) {	// test si le pixel existe i-taille/2
				int dk = max(0, i - ns2);
				int fk = min(i + ns2, this->lireHauteur() - 1);
				int dl = max(0, j - ms2);
				int fl = min(j + ms2, this->lireLargeur() - 1);

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

	double hough_h = max(this->lireHauteur() / 2, this->lireLargeur() / 2)*sqrt(2.0);

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
	CImageDouble mL = this->maxiLocaux(N,M);
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
		std::cout << nbLignes-1 << " lignes avec les infos suivants :" << std::endl;
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

	CImageNdg HI(img.lireHauteur(),img.lireLargeur(),0);
	HI.ecrireNom(img.lireNom()+"HI");
	HI.choixPalette(img.lirePalette());

	double hough_h = max(img.lireHauteur() / 2, img.lireLargeur() / 2)*sqrt(2.0);

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