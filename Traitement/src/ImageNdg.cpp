#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <cmath>
#include <vector>
#include <algorithm>

#include "ImageNdg.h"
#include "ImageClasse.h"

#define PI 3.14159265358979323846
#define MAGIC_NUMBER_BMP ('B'+('M'<<8)) // signature bitmap windows

// constructeurs et destructeur
CImageNdg::CImageNdg() {

	this->m_iHauteur  = 0;
	this->m_iLargeur  = 0;
	this->m_bBinaire  = false;
	this->m_sNom      = "vide";
	
	this->m_pucPixel  = NULL;
	this->m_pucPalette = NULL; 
}

CImageNdg::CImageNdg(int hauteur, int largeur, int valeur) {

	this->m_iHauteur = hauteur;
	this->m_iLargeur = largeur;
	this->m_bBinaire	= false; // Image Ndg par défaut, binaire après seuillage
	this->m_sNom      = "inconnu";

	this->m_pucPixel = new unsigned char[hauteur*largeur];
	this->m_pucPalette = new unsigned char[256*4];	
	choixPalette("grise"); // palette grise par défaut, choix utilisateur 
	if (valeur != -1) 
		for (int i=0;i<this->lireNbPixels();i++)
			this->m_pucPixel[i] = valeur;
}

CImageNdg::CImageNdg(const std::string& name) {

	BITMAPFILEHEADER header;
	BITMAPINFOHEADER infoHeader;
	
	std::ifstream f(name.c_str(),std::ios::in | std::ios::binary); 
		if (f.is_open()) {
			f.read((char*)&header,sizeof(BITMAPFILEHEADER));
			if (header.bfType != MAGIC_NUMBER_BMP) 
				throw std::string("ouverture format BMP impossible ..."); 
			else {
				f.read((char*)&infoHeader,sizeof(BITMAPINFOHEADER));
				if (infoHeader.biCompression > 0) 
					throw std::string("Format compresse non supporte...");
				else {
					if (infoHeader.biBitCount == 8) {
						this->m_iHauteur = infoHeader.biHeight;
						this->m_iLargeur = infoHeader.biWidth;
						this->m_bBinaire = false;
						this->m_sNom.assign(name.begin(),name.end()-4);
						this->m_pucPalette = new unsigned char[256*4];	
						this->m_pucPixel = new unsigned char[infoHeader.biHeight * infoHeader.biWidth];

						// gérer multiple de 32 bits via zéros éventuels ignorés
						int complement = (((this->m_iLargeur-1)/4) + 1)*4 - this->m_iLargeur;
						for (int indice=0;indice<4*256;indice++) 
							f.read((char*)&this->m_pucPalette[indice],sizeof(char));

						for (int i= this->m_iHauteur-1; i >= 0; i--) {
							for (int j=0; j<this->m_iLargeur; j++) 
								f.read((char*)&this->m_pucPixel[i*this->m_iLargeur+j],sizeof(char));

							char inutile;
							for (int k=0; k< complement; k++)
								f.read((char*)&inutile,sizeof(char));
						}
					}	
					else {
						// cas d'une image couleur
						this->m_iHauteur = infoHeader.biHeight;
						this->m_iLargeur = infoHeader.biWidth;
						this->m_bBinaire = false;
						this->m_sNom.assign(name.begin(),name.end()-4);
						this->m_pucPalette = new unsigned char[256*4];	
						this->choixPalette("grise"); // palette grise par défaut
						this->m_pucPixel = new unsigned char[infoHeader.biHeight * infoHeader.biWidth];

						// extraction plan luminance
						int complement = (((this->m_iLargeur*3-1)/4) + 1)*4 - this->m_iLargeur*3;
						for (int i= this->m_iHauteur-1; i >= 0; i--) {
							for (int j=0;j<this->m_iLargeur*3;j+=3) {
								unsigned char rouge,vert,bleu;
								f.read((char*)&rouge,sizeof(char));
								f.read((char*)&vert,sizeof(char)); 
								f.read((char*)&bleu,sizeof(char));
								this->m_pucPixel[i*this->m_iLargeur+j/3]=(unsigned char)(((int)rouge+(int)vert+(int)bleu)/3);
							}	

							char inutile;
							for (int k=0; k< complement; k++)
								f.read((char*)&inutile,sizeof(char));
						}
					}
				}
			}
			f.close();
		}
		else
			throw std::string("ERREUR : Image absente (ou pas ici en tout cas) !");
}

CImageNdg::CImageNdg(const CImageNdg& im) {

	this->m_iHauteur = im.lireHauteur();
	this->m_iLargeur = im.lireLargeur();
	this->m_bBinaire = im.lireBinaire(); 
	this->m_sNom     = im.lireNom();
	this->m_pucPixel = NULL; 
	this->m_pucPalette = NULL;

	if (im.m_pucPalette != NULL) {
		this->m_pucPalette = new unsigned char[256*4];
		memcpy(this->m_pucPalette,im.m_pucPalette,4*256);
	}
	if (im.m_pucPixel != NULL) {
		this->m_pucPixel = new unsigned char[im.lireHauteur() * im.lireLargeur()];
		memcpy(this->m_pucPixel,im.m_pucPixel,im.lireNbPixels());
	}
}

CImageNdg::~CImageNdg() {
	if (this->m_pucPixel) {
		delete[] this->m_pucPixel;
		this->m_pucPixel = NULL;
	}

	if (this->m_pucPalette) {
		delete[] this->m_pucPalette;
		this->m_pucPalette = NULL;
	}
}

void CImageNdg::sauvegarde(const std::string& fixe) {
	BITMAPFILEHEADER header;
	BITMAPINFOHEADER infoHeader;

	if (this->m_pucPixel) {
		std::string nomFichier = "../Res/";
		if (fixe.compare("") == 0)
			nomFichier += this->lireNom() + ".bmp"; // force sauvegarde dans répertoire Res (doit exister)
		else
			nomFichier += fixe;

		std::ofstream f(nomFichier.c_str(),std::ios::binary);
		if (f.is_open()) {

			int complement = (((this->m_iLargeur-1)/4) + 1)*4 - this->m_iLargeur;

			header.bfType = MAGIC_NUMBER_BMP;
			header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 4*256*sizeof(char); // palette
			header.bfSize = header.bfOffBits + (complement+this->lireNbPixels()*sizeof(char));
			header.bfReserved1 = 0;
			header.bfReserved2 = 0;
			f.write((char*)&header,sizeof(BITMAPFILEHEADER));
			
			infoHeader.biHeight = this->m_iHauteur;
			infoHeader.biWidth = this->m_iLargeur;
			infoHeader.biCompression = 0;
			infoHeader.biClrUsed = 0;
			infoHeader.biBitCount = 8;
			infoHeader.biSizeImage = 0; //pas de compression;
			infoHeader.biClrUsed = 256;
			infoHeader.biClrImportant = 256;
			infoHeader.biHeight = this->m_iHauteur;
			infoHeader.biWidth = this->m_iLargeur;
			infoHeader.biPlanes = 1;
			infoHeader.biSize = sizeof(infoHeader);
			infoHeader.biSizeImage = this->lireNbPixels();
			f.write((char*)&infoHeader,sizeof(BITMAPINFOHEADER));

			// on remplit la palette
			for (int indice=0;indice<4*256;indice ++) 
				f.write((char*)&this->m_pucPalette[indice],sizeof(char)); 

			for (int i= this->m_iHauteur-1; i >= 0; i--) {
				for (int j=0;j<m_iLargeur;j++)    
					f.write((char*)&this->m_pucPixel[i*m_iLargeur+j],sizeof(char));
					
				// gérer multiple de 32 bits
				char inutile;
				for (int k=0; k< complement; k++)
					f.write((char*)&inutile,sizeof(char)); 
				}
		f.close();
		}
		else
			throw std::string("Impossible de creer le fichier de sauvegarde !");
	}
	else
		throw std::string("Pas de donnee a sauvegarder !");
}

CImageNdg& CImageNdg::operator=(const CImageNdg& im) {

	if (&im == this)
		return *this;

	this->m_iHauteur = im.lireHauteur();
	this->m_iLargeur = im.lireLargeur();
	this->m_bBinaire = im.lireBinaire(); 
	this->m_sNom     = im.lireNom();

	if (this->m_pucPixel) 
		delete[] this->m_pucPixel;
	this->m_pucPixel = new unsigned char[this->m_iHauteur * this->m_iLargeur];

	if (this->m_pucPalette)
		delete[] this->m_pucPalette;
	this->m_pucPalette = new unsigned char[256*4];

	if (im.m_pucPalette != NULL)
		memcpy(this->m_pucPalette,im.m_pucPalette,4*256);
	if (im.m_pucPixel != NULL)
		memcpy(this->m_pucPixel,im.m_pucPixel,im.lireNbPixels());

return *this;
}

// fonctionnalités histogramme 

std::vector<unsigned long> CImageNdg::histogramme(bool enregistrementCSV, int pas) {

	std::vector<unsigned long> h;

	h.resize(256/pas,0);
	for (int i=0;i<this->lireNbPixels();i++) 
		h[this->operator()(i)/pas] += 1L;

	if (enregistrementCSV) {
	 std::string fichier = "../Res/" + this->lireNom() + ".csv";
		std::ofstream f (fichier.c_str());

		if (!f.is_open())
			std::cout << "Impossible d'ouvrir le fichier en ecriture !" << std::endl;
		else {
			for (int i=0;i<(int)h.size();i++)
				f << h[i] << std::endl;
		}
		f.close();
	}

	return h;
}

// Histogramme normalisé

std::vector<double> CImageNdg::histogramme_norm(bool enregistrementCSV, int pas) {

	std::vector<double> h;

	h.resize(256 / pas, 0);
	for (int i = 0; i<this->lireNbPixels(); i++)
		h[this->operator()(i) / pas] += 1L;

	for (int i = 0; i < 256; i++)
		h[i] = h[i] / this->lireNbPixels();

	if (enregistrementCSV) {
		std::string fichier = "../Res/" + this->lireNom() + ".csv";
		std::ofstream f(fichier.c_str());

		if (!f.is_open())
			std::cout << "Impossible d'ouvrir le fichier en ecriture !" << std::endl;
		else {
			for (int i = 0; i<(int)h.size(); i++)
				f << h[i] << std::endl;
		}
		f.close();
	}

	return h;
}

// signatures globales

MOMENTS CImageNdg::signatures(const std::vector<unsigned long>& h) {

	MOMENTS globales;
	
	// min
	int i=0;
	while ((i < (int)h.size()) && (h[i] == 0))
		i++;
	globales.minNdg = i;
		
	// max
	i=h.size()-1;
	while ((i > 0) && (h[i] == 0))
		i--;
	globales.maxNdg = i;

	// mediane

	int moitPop = this->lireNbPixels()/2;

	i=globales.minNdg;
	int somme = h[i];
	while (somme < moitPop) {
		i += 1;
		if (i < (int)h.size())
			somme += h[i];
	}
	globales.mediane = i;

	// moyenne et écart-type
	double moy=0,sigma=0;
	for (i=globales.minNdg;i<=globales.maxNdg;i++) {
		moy += ((double)h[i])*i;
		sigma += ((double)h[i])*i*i;
	}
	moy /= (double)this->lireNbPixels();
	sigma = sqrt(sigma/(double)this->lireNbPixels() - (moy*moy));
	globales.moyenne = moy;
	globales.ecartType = sigma;

	return globales;
}

MOMENTS CImageNdg::signatures() {
	
	MOMENTS globales;
	std::vector<unsigned long> hist;
	hist=this->histogramme();

	globales = this->signatures(hist);
	return globales;
}

// opérations ensemblistes images binaires
CImageNdg CImageNdg::operation(const CImageNdg& im, const std::string& methode) {

	if ((&im == this) || !(this->lireBinaire() && im.lireBinaire())) {
		return *this;
	}

	CImageNdg out(this->lireHauteur(), this->lireLargeur());
	out.m_bBinaire = this->lireBinaire(); 
	out.m_sNom     = this->lireNom()+"Op";
	out.choixPalette("binaire"); // palette binaire par défaut pour img binaire

	if (methode.compare("et") == 0) {
		for (int i=0;i<this->lireNbPixels();i++)
			out(i) = this->operator()(i) && im(i);
	}
	else
		if (methode.compare("ou") == 0) {
			for (int i=0;i<this->lireNbPixels();i++)
				out(i) = this->operator()(i) || im(i);
		}
			if (methode.compare("-") == 0) {
				for (int i = 0; i<this->lireNbPixels(); i++)
					out(i) = this->operator()(i) - im(i); // vigilance sur les images opérérées !
			}

return out;
}


// ancien seuillage
//CImageNdg CImageNdg::seuillage(const std::string& methode, int& seuilBas, int& seuilHaut) {
//	
//	if (!this->m_bBinaire) {
//		CImageNdg out(this->lireHauteur(),this->lireLargeur());
//		out.m_sNom     = this->lireNom()+"S";
//		out.choixPalette("binaire"); // palette binaire par défaut
//		out.m_bBinaire = true;
//		seuilBas = 128;
//		seuilHaut = 255;
//
//		// création lut pour optimisation calcul
//		std::vector<int> lut;
//		lut.resize(256);
//
//		// recherche valeur seuil
//		// cas "manuel" -> seuil reste celui passé en paramètre
//
//		if (methode.compare("automatique") == 0) 
//		{
//			std::vector<unsigned long> hist = this->histogramme();
//			std::vector<unsigned long> histC; // histogramme cumulé
//			histC.resize(256,0);
//			histC[0] = hist[0];
//			for (int i=1;i<(int)hist.size();i++) 
//				histC[i] = histC[i-1]+hist[i];
//
//			MOMENTS globales = this->signatures(hist);
//			int min = globales.min,
//				max = globales.max;
//
//			// f(s)
//			std::vector<double> tab;
//			tab.resize(256,0);
//		
//			double M1,M2;
//		
//			// initialisation
//			M1 = (double)min;
//			double nb=0;
//			M2=0;
//			for (int i=min+1;i<=max;i++) {
//				M2 += (double)hist[i]*i;
//				nb += (double)hist[i];
//			}
//			if (nb > 0)
//				M2 /= nb;
//			tab[min] = fabs(min - (M1 + M2)/2);
//		
//			// parcours
//			for (int i=min+1;i<=max;i++) {
//				M1 = ( (double)histC[i-1]*M1 + hist[i]*i ) / histC[i];
//				M2 = ( (double)(histC[max]-histC[i-1])*M2 - hist[i]*i) / (histC[max]-histC[i]);
//				tab[i] = fabs(i - (M1 + M2)/2);
//			}
//
//			// recherche s
//			seuilBas = min;
//			seuilHaut = 255;
//			for (int i=min+1;i<=max;i++)
//				if (tab[i] < tab[seuilBas])
//					seuilBas = i;
//		}
//
//		// fin recherche valeur seuil 
//
//		// génération lut
//		for (int i = 0; i < seuilBas; i++)
//			lut[i] =  0; 
//		for (int i = seuilBas; i <= seuilHaut; i++)
//			lut[i] = 1;
//		for (int i = seuilHaut+1; i <= 255; i++)
//			lut[i] = 0;
//
//		// création image seuillée
//		std::cout << "Seuillage des pixels entre " << seuilBas << " et " << seuilHaut << std::endl;
//		for (int i=0; i < out.lireNbPixels(); i++) 
//			out(i) = lut[this->operator ()(i)]; 
//
//		return out;
//		}
//	else {
//		std::cout << "Seuillage image binaire impossible" << std::endl;
//		return (*this);
//	}
//}

// seuillage
CImageNdg CImageNdg::seuillage(const std::string& methode, int seuilBas, int seuilHaut) {

	if (!this->m_bBinaire) {
		CImageNdg out(this->lireHauteur(), this->lireLargeur());
		out.choixPalette("binaire"); // palette binaire par défaut
		out.m_bBinaire = true;

		// création lut pour optimisation calcul
		std::vector<int> lut;
		lut.resize(256);

		// recherche valeur seuil
		// cas "manuel" 
		if (methode.compare("manuel") == 0)
		{
			out.m_sNom = this->lireNom() + "SeMa";
		}
		else
			if (methode.compare("otsu") == 0)
			{
				out.m_sNom = this->lireNom() + "SeAu";
				// recherche seuil via Otsu
				std::vector<unsigned long> hist = this->histogramme();
				std::vector<unsigned long> histC; // histogramme cumulé
				histC.resize(256, 0);
				histC[0] = hist[0];
				for (int i = 1; i < (int)hist.size(); i++)
					histC[i] = histC[i - 1] + hist[i];

				MOMENTS globales = this->signatures(hist);
				int min = globales.minNdg,
					max = globales.maxNdg;

				// f(s)
				std::vector<double> tab;
				tab.resize(256, 0);

				double M1, M2, w1;

				// initialisation
				M1 = min;
				seuilBas = min;
				seuilHaut = 255;

				w1 = (double)histC[min] / (double)(this->lireNbPixels());
				M2 = 0;
				for (int i = min + 1; i <= max; i++)
					M2 += (double)hist[i] * i;
				M2 /= (double)(histC[max] - hist[min]);
				tab[min] = w1*(1 - w1)*(M1 - M2)*(M1 - M2);

				for (int i = min + 1; i < max; i++) {
					M1 = ((double)histC[i - 1] * M1 + (double)hist[i] * i) / histC[i];
					M2 = ((double)(histC[255] - histC[i - 1])*M2 - hist[i] * i) / (double)(histC[255] - histC[i]);
					w1 = (double)histC[i] / (double)(this->lireNbPixels());
					tab[i] = w1*(1 - w1)*(M1 - M2)*(M1 - M2);
					if (tab[i] > tab[seuilBas])
						seuilBas = i;
				}
			}
			else {
				MOMENTS globales = { 0,0,0,0,0 };

				std::vector<unsigned long> hist;
				hist = this->histogramme();

				globales = this->signatures(hist);
				if (methode.compare("moyenne") == 0)
				{
					out.m_sNom = this->lireNom() + "SeMo";
					seuilBas = (int)globales.moyenne;
				}
				else
					if (methode.compare("mediane") == 0)
					{
						out.m_sNom = this->lireNom() + "SeMe";
						seuilBas = globales.mediane;
					}
			}


			// génération lut
			for (int i = 0; i < seuilBas; i++)
				lut[i] = 0;
			for (int i = seuilBas; i <= seuilHaut; i++)
				lut[i] = 1;
			for (int i = seuilHaut + 1; i <= 255; i++)
				lut[i] = 0;

			// création image seuillée
			std::cout << "Seuillage des pixels entre " << seuilBas << " et " << seuilHaut << std::endl;
			for (int i = 0; i < out.lireNbPixels(); i++)
				out(i) = lut[this->operator ()(i)];

			return out;
	}
	else {
		throw std::string("Seuillage image binaire impossible");
		return (*this);
	}
}


// transformation

CImageNdg CImageNdg::transformation(const std::string& methode,int vMinOut, int vMaxOut) {

	CImageNdg out(this->lireHauteur(),this->lireLargeur());
	out.m_sNom     = this->lireNom()+"T";
	out.choixPalette(this->lirePalette()); // conservation de la palette
	out.m_bBinaire = this->m_bBinaire; // conservation du type

	if (methode.compare("complement") == 0) {
		if (!this->m_bBinaire) {
			// ndg -> 255-ndg
			// création lut pour optimisation calcul
			std::vector<int> lut;
			lut.resize(256);

			for (int i=0; i < 256; i++)
				lut[i] = (int)(255 - i);
			for (int i=0; i < out.lireNbPixels(); i++) 
				out(i) = lut[this->operator()(i)]; 
		}
		else {
			// 0 -> 1 et 1 -> 0
			for (int i=0; i < out.lireNbPixels(); i++) 
				out(i) = !this->operator()(i);
		}
	}
	else
		if (methode.compare("expansion") == 0) {
			std::vector<unsigned long> h = this->histogramme(false); 
			// recherche min et max image
			int min = 0,max = 255;
			int i=0;
			while (h[i] == 0)
				i++;
			min = i;
			i = 255;
			while (h[i] == 0)
				i--;
			max = i;
	
			if (max > min) {	
				double a=(double)(vMaxOut-vMinOut)/(double)(max-min);
				double b=(double)vMinOut-a*min;

				std::vector<int> lut;
				lut.resize(256);

				for (int i=min; i<=max; i++)
					lut[i] = (int)(a*i+b);

				std::cout << "Expansion dynamique [" << min << " - " << max << "] vers [" << vMinOut << " - " << vMaxOut << "]" << std::endl;
				for (int i=0; i < out.lireNbPixels(); i++) 
					out(i) = lut[this->operator()(i)]; 
			}
			else 
				for (i=0;i<this->lireNbPixels();i++)
					out(i) = this->operator()(i);
		}
		else
			if (methode.compare("egalisation") == 0) {
					std::vector<unsigned long> h = this->histogramme(false); 
					std::vector<unsigned long> hC = h;
					for (int i=1;i<(int)h.size();i++)
						hC[i] = hC[i-1] + h[i];
					
					// recherche min et max image
					int min = 0,max = 255;
					int i=0;
					while (h[i] == 0)
						i++;
					min = i;
					i = 255;
					while (h[i] == 0)
						i--;
					max = i;

					std::vector<int> lut;
					lut.resize(256);

					for (int i=min; i<=max; i++)
						lut[i] = (int)( ((double)hC[i] / (double)this->lireNbPixels() )*(double)255 );

					std::cout << "Egalisation histogramme sur [" << min << " - " << max << "] vers [0 - 255]" << std::endl;
					for (int i=0; i < out.lireNbPixels(); i++) 
						out(i) = lut[this->operator()(i)]; 
			}

	return out;
}

// morphologie
		
CImageNdg CImageNdg::morphologie(const std::string& methode, const std::string& eltStructurant) {
		
	CImageNdg out(this->lireHauteur(),this->lireLargeur());
	out.m_sNom     = this->lireNom()+"M";
	out.choixPalette(this->lirePalette()); // conservation de la palette
	out.m_bBinaire = this->m_bBinaire; // conservation du type
		
	if (methode.compare("erosion") == 0) {	
		CImageNdg agrandie(this->lireHauteur()+2,this->lireLargeur()+2);

		// gestion des bords
		if (this->lireBinaire()) {
			int pix;

			for (pix=0;pix<agrandie.lireLargeur();pix++) {
				agrandie(0,pix) = 1;
				agrandie(this->lireHauteur()-1,pix) = 1;
			}
			for (pix=1;pix<agrandie.lireHauteur()-1;pix++) {
				agrandie(pix,0) = 1;
				agrandie(pix,this->lireLargeur()-1) = 1;
			}
		}
		else {
			int pix;

			for (pix=0;pix<agrandie.lireLargeur();pix++) {
				agrandie(0,pix) = 255;
				agrandie(this->lireHauteur()-1,pix) = 255;
			}
			for (pix=1;pix<agrandie.lireHauteur()-1;pix++) {
				agrandie(pix,0) = 255;
				agrandie(pix,this->lireLargeur()-1) = 255;
			}
		}

		// gestion du coeur
		for (int i=0;i<this->lireHauteur();i++)
			for (int j=0;j<this->lireLargeur();j++) {
				agrandie(i+1,j+1)=this->operator()(i,j);
			}

		if (eltStructurant.compare("V4") == 0) {
			for (int i=1;i<agrandie.lireHauteur()-1;i++)
				for (int j=1;j<agrandie.lireLargeur()-1;j++) {
					int minH = min(agrandie(i,j-1),agrandie(i,j+1));
					int minV = min(agrandie(i-1,j),agrandie(i+1,j));
					int minV4 = min(minH,minV);
					out(i-1,j-1)=min(minV4,agrandie(i,j));
				}
		}
		else {
			if (eltStructurant.compare("V8") == 0) {
			for (int i=1;i<agrandie.lireHauteur()-1;i++)
				for (int j=1;j<agrandie.lireLargeur()-1;j++) {
					int minH = min(agrandie(i,j-1),agrandie(i,j+1));
					int minV = min(agrandie(i-1,j),agrandie(i+1,j));
					int minV4 = min(minH,minV);
					int minD1 = min(agrandie(i-1,j-1),agrandie(i+1,j+1));
					int minD2 = min(agrandie(i-1,j+1),agrandie(i+1,j-1));
					int minD = min(minD1,minD2);
					int minV8 = min(minV4,minD);
					out(i-1,j-1)=min(minV8,agrandie(i,j));
				}
			}
		}
	}
	else {
		if (methode.compare("dilatation") == 0) {	
			CImageNdg agrandie(this->lireHauteur()+2,this->lireLargeur()+2);

			// gestion des bords
			int pix;

			for (pix=0;pix<agrandie.lireLargeur();pix++) {
				agrandie(0,pix) = 0;
				agrandie(agrandie.lireHauteur()-1,pix) = 0;
			}
			for (pix=1;pix<agrandie.lireHauteur()-1;pix++) {
				agrandie(pix,0) = 0;
				agrandie(pix,agrandie.lireLargeur()-1) = 0;
			}

			// gestion du coeur
			for (int i=0;i<this->lireHauteur();i++)
				for (int j=0;j<this->lireLargeur();j++) {
					agrandie(i+1,j+1)=this->operator()(i,j);
				}

			if (eltStructurant.compare("V4") == 0) {
				for (int i=1;i<agrandie.lireHauteur()-1;i++)
					for (int j=1;j<agrandie.lireLargeur()-1;j++) {
						int maxH = max(agrandie(i,j-1),agrandie(i,j+1));
						int maxV = max(agrandie(i-1,j),agrandie(i+1,j));
						int maxV4 = max(maxH,maxV);
						out(i-1,j-1)=max(maxV4,agrandie(i,j));
					}
			}
			else {
				if (eltStructurant.compare("V8") == 0) {
				for (int i=1;i<agrandie.lireHauteur()-1;i++)
					for (int j=1;j<agrandie.lireLargeur()-1;j++) {
						int maxH = max(agrandie(i,j-1),agrandie(i,j+1));
						int maxV = max(agrandie(i-1,j),agrandie(i+1,j));
						int maxV4 = max(maxH,maxV);
						int maxD1 = max(agrandie(i-1,j-1),agrandie(i+1,j+1));
						int maxD2 = max(agrandie(i-1,j+1),agrandie(i+1,j-1));
						int maxD = max(maxD1,maxD2);
						int maxV8 = max(maxV4,maxD);
						out(i-1,j-1)=max(maxV8,agrandie(i,j));
					}
				}
			}
		}
	}

	return out;
}

//uniquement image binaires
CImageNdg CImageNdg::bouchageTrous()
{
	CImageNdg ImgOut =CImageNdg(this->lireHauteur(), this->lireLargeur());
	ImgOut.m_bBinaire = true;
	CImageNdg ImExp = CImageNdg(this->lireHauteur()+2, this->lireLargeur()+2,0);
	ImExp.m_bBinaire = true;
	CImageNdg ImCompl = CImageNdg(this->lireHauteur()+2, this->lireLargeur()+2, 1);
	CImageClasse ImgFill;
	std::vector<int> LUT = std::vector<int>(256,1);

	for (int i = 0; i <this->lireLargeur(); i++)
	{
		for (int j = 0; j < this->lireLargeur(); j++)
		{
			ImExp(i + 1, j + 1) = this->operator()(i, j);
			ImCompl(i + 1, j + 1) -= this->operator()(i, j);
		}
	}

	ImgFill = CImageClasse(ImCompl,"V8");
	LUT.at(0) = 0;
	LUT.at(1) = 0;

	for (int i = 0; i < this->lireHauteur(); i++)
	{
		for (int j = 0; j < this->lireLargeur(); j++)
		{
			ImgOut(i, j) = LUT.at(ImgFill(i + 1, j + 1));
		}
	}

	ImgOut = this->operation(ImgOut, "ou");
	return ImgOut;
}


// filtrage : moyennage ou median
CImageNdg CImageNdg::filtrage(const std::string& methode, int Ni, int Nj) {
		
	CImageNdg out(this->lireHauteur(),this->lireLargeur());
	out.m_sNom     = this->lireNom()+"F";
	out.choixPalette(this->lirePalette()); // conservation de la palette
	out.m_bBinaire = this->m_bBinaire; // conservation du type
		
	if (methode.compare("moyennage") == 0) {	
		int nbBordsi = Ni/2;
		int nbBordsj = Nj/2;

		for (int i=0;i<this->lireHauteur();i++)
			for (int j=0;j<this->lireLargeur();j++) {
				// gestion des bords
				int dk=max(0,i-nbBordsi);
				int fk=min(i+nbBordsi,this->lireHauteur()-1);
				int dl=max(0,j-nbBordsj);
				int fl=min(j+nbBordsj,this->lireLargeur()-1);

				float somme=0;
				float moy=0;
				for (int k=dk;k<=fk;k++)
					for (int l=dl;l<=fl;l++) {
						moy += (float)this->operator()(k,l);
						somme += 1;
					}
				out(i,j)=(int)(moy/somme);
			}
	}
	else
		if (methode.compare("median") == 0) {
			int nbBordsi = Ni / 2;
			int nbBordsj = Nj / 2;

			std::vector<int> voisinage;

			for (int i = 0; i<this->lireHauteur(); i++)
				for (int j = 0; j<this->lireLargeur(); j++) {
					// gestion des bords
					int dk = max(0, i - nbBordsi);
					int fk = min(i + nbBordsi, this->lireHauteur() - 1);
					int dl = max(0, j - nbBordsj);
					int fl = min(j + nbBordsj, this->lireLargeur() - 1);

					voisinage.resize((fk - dk + 1)*(fl - dl + 1));
					int indMed = (fk - dk + 1)*(fl - dl + 1) / 2;

					// empilement 
					int indice = 0;
					for (int k = dk; k <= fk; k++)
						for (int l = dl; l <= fl; l++) {
							voisinage.at(indice) = (int)this->operator()(k, l);
							indice++;
						}

					// tri croissant
					std::sort(voisinage.begin(), voisinage.end());

					out(i, j) = voisinage.at(indMed);

					voisinage.clear();
				}
		}

	return out;
}

CImageNdg CImageNdg::rotation(float angle,const std::string& taille) 
{
	int ithis, jthis;
	float icenter, jcenter;
	float costhe = (float)cos(-PI*angle / 180);
	float sinthe = (float)sin(-PI*angle / 180);

	if (taille.compare("idem") == 0) {
		CImageNdg out(this->lireHauteur(), this->lireLargeur(),this->operator()(0)); //on prend la valeur "approchée" du fond
		out.ecrireNom(this->lireNom() + "Rot");

		//centrage de l'image d'origine
		icenter = (float)(this->lireHauteur() / 2.0);
		jcenter = (float)(this->lireLargeur() / 2.0);

		for (int i = 0; i < this->lireHauteur(); i++)
			for (int j = 0; j < this->lireLargeur(); j++)
			{
				ithis = (int)(costhe*(i - icenter) + sinthe*(j - jcenter)+icenter);
				jthis = (int)(-sinthe*(i - icenter) + costhe*(j - jcenter) + jcenter);
				if (ithis >= 0 && ithis < out.lireHauteur() && jthis >= 0 && jthis < out.lireLargeur())
					out(i, j) = this->operator()(ithis, jthis);
			}
		return out;
	}
	CImageNdg vide;
	vide.ecrireNom("Taille non bien définie!");
	return vide;
}

CImageNdg CImageNdg::rognageSigComposante(const std::string& methode, int seuilBas, int seuilHaut)
{
	//
	//labellisation si plusieurs pièces après
	//CImageClasse imgLab(img_seuil,"V8");
	//std::vector<SIGNATURE_Forme> sig = imgLab.sigComposantesConnexes(false);

	////recherche du plus gros blob
	//int indmax = 0;
	//for (unsigned int i = 0; i < sig.size(); i++)
	//	if (indmax < sig[i].surface)
	//		indmax = i;
	//SIGNATURE_Forme sigPiece = sig[indmax];

	//rectangle englobant seul sur image seuillée en supposant une unique pièce
	CImageNdg img_seuil = this->seuillage(methode, seuilBas, seuilHaut);
	img_seuil = img_seuil.morphologie("erosion", "V8").morphologie("dilatation", "V8");
	
	SIGNATURE_Forme sigPiece = { 0,0,0,"0",0,0,0,0,0,0,0};
	int n = 0;
	int trouveHj = false;
	int trouveBj = false;
	int trouveHi = false;
	int trouveBi = false;
	while (n<img_seuil.lireNbPixels() && (!trouveBj || !trouveHj || !trouveBi || !trouveHi))
	{
		if (!trouveHi && img_seuil(n))
		{
			sigPiece.rectEnglob_Hi = n / img_seuil.lireLargeur();
			trouveHi = true;
		}
		if (!trouveBi && img_seuil(img_seuil.lireNbPixels() - 1 - n))
		{
			sigPiece.rectEnglob_Bi = (img_seuil.lireNbPixels() - 1 - n) / img_seuil.lireLargeur();
			trouveBi = true;
		}
		if (!trouveHj && img_seuil(n%img_seuil.lireHauteur(), n / img_seuil.lireHauteur()))
		{
			sigPiece.rectEnglob_Hj = n / img_seuil.lireHauteur();
			trouveHj = true;
		}
		if (!trouveBj && img_seuil((img_seuil.lireNbPixels() - 1 - n) % img_seuil.lireHauteur(), (img_seuil.lireNbPixels() - 1 - n) / img_seuil.lireHauteur()))
		{
			sigPiece.rectEnglob_Bj = (img_seuil.lireNbPixels() - 1 - n) / img_seuil.lireHauteur();
			trouveBj = true;
		}
		n++;
	}

	//Detection de la pièce dans l'espace
	CImageNdg imgRogne(sigPiece.rectEnglob_Bi - sigPiece.rectEnglob_Hi, sigPiece.rectEnglob_Bj - sigPiece.rectEnglob_Hj);

	for (int i = 0; i<imgRogne.lireHauteur(); i++)
		for (int j = 0; j < imgRogne.lireLargeur(); j++)
			imgRogne(i, j) = this->operator()(i + sigPiece.rectEnglob_Hi, j + sigPiece.rectEnglob_Hj);


	/* pas sûr unsafe je connais pas le comportement*/
		if (imgRogne.lireHauteur() > 350 || imgRogne.lireLargeur() > 300)
		{
			CImageNdg img_seuilRogne = imgRogne.seuillage(methode, seuilBas, seuilHaut);
			
			int Gh = img_seuilRogne.lireHauteur() / 3;
			int Gb = 2 * img_seuilRogne.lireHauteur() / 3;
			int Dh = img_seuilRogne.lireHauteur() / 3;
			int Db = 2 * img_seuilRogne.lireHauteur() / 3;
			bool tGh = false, tGb = false, tDh = false, tDb = false;
			int Ghj, Gbj, Dhj, Dbj;
			int j = 0;
			while ((!tGb || !tGh || !tDh || !tDb) && j<img_seuilRogne.lireLargeur()){
				if (!tGb && img_seuilRogne(Gb, j)){	tGb = true;	Gbj = j;}
				if (!tGh && img_seuilRogne(Gh, j)){	tGh = true;	Ghj = j;}
				if (!tDb && img_seuilRogne(Db, img_seuilRogne.lireLargeur() - 1 - j)){	tDb = true;	Dbj = img_seuilRogne.lireLargeur() - 1 - j;}
				if (!tDh && img_seuilRogne(Dh, img_seuilRogne.lireLargeur() - 1 - j)){	tDh = true;	Dhj = img_seuilRogne.lireLargeur() - 1 - j;}
				j++;
			}
			
			int Hg = img_seuilRogne.lireLargeur() / 3;
			int Hd = 2 * img_seuilRogne.lireLargeur() / 3;
			int Bg = img_seuilRogne.lireLargeur() / 3;
			int Bd = 2 * img_seuilRogne.lireLargeur() / 3;
			bool tHg = false, tHd = false, tBg = false, tBd = false;
			int Hgi, Hdi, Bgi, Bdi;
			int i = 0;
			while ((!tHd || !tHg || !tBg || !tBd) && i<img_seuilRogne.lireHauteur()){
				if (!tHd && img_seuilRogne(i, Hd)) { tHd = true;	Hdi = i; }
				if (!tHg && img_seuilRogne(i, Hg)) { tHg = true;	Hgi = i; }
				if (!tBd && img_seuilRogne(img_seuilRogne.lireHauteur() - 1 - i, Bd)) { tBd = true;	Bdi = img_seuilRogne.lireHauteur() - 1 - i; }
				if (!tBg && img_seuilRogne(img_seuilRogne.lireHauteur() - 1 - i, Bg)) { tBg = true;	Bgi = img_seuilRogne.lireHauteur() - 1 - i; }
				i++;
			}
			int Gj = max(Gbj, Ghj);
			int Dj = min(Dbj, Dhj);
			int Hi = max(Hdi, Hgi);
			int Bi = min(Bdi, Bgi);
	
			CImageNdg imgFinale(abs(Hi - Bi), abs(Dj - Gj));
			for (int i = 0; i<imgFinale.lireHauteur(); i++)
				for (int j = 0; j < imgFinale.lireLargeur(); j++)
					imgFinale(i, j) = imgRogne(i + Hi, j + Gj);
			return imgFinale;
		}

	return imgRogne;
}