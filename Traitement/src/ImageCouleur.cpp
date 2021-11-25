#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>
#include <cmath>
#include <vector>

#include "ImageCouleur.h"

#define MAGIC_NUMBER_BMP ('B'+('M'<<8)) // signature bitmap windows

// constructeurs et destructeur
CImageCouleur::CImageCouleur() {

	this->m_iHauteur  = 0;
	this->m_iLargeur  = 0;
	this->m_sNom      = "vide";
	this->m_pucData   = NULL;
	this->m_ppucPixel = NULL;
}

CImageCouleur::CImageCouleur(int hauteur, int largeur, int valR, int valV, int valB) {

	this->m_iHauteur = hauteur;
	this->m_iLargeur = largeur;
	this->m_sNom     = "inconnu";

	this->m_pucData = new unsigned char[hauteur*largeur*3];
	this->m_ppucPixel = new unsigned char*[hauteur*largeur];
	for (int i=0;i<hauteur*largeur;i++)
		this->m_ppucPixel[i] = &this->m_pucData[3*i];

	if (valR != -1) 
		for (int i=0;i<this->lireNbPixels();i++)
			this->m_ppucPixel[i][0] = valR;
	if (valV != -1) 
		for (int i=0;i<this->lireNbPixels();i++)
			this->m_ppucPixel[i][1] = valV;
	if (valB != -1) 
		for (int i=0;i<this->lireNbPixels();i++)
			this->m_ppucPixel[i][2] = valB;
}

CImageCouleur::CImageCouleur(const std::string& name) {

	BITMAPFILEHEADER header;
	BITMAPINFOHEADER infoHeader;
	
	std::ifstream f(name.c_str(),std::ios::in | std::ios::binary); // transformation d'une string en chaîne de type C
	if (f.is_open()) {
		f.read((char*)&header,sizeof(BITMAPFILEHEADER));
		if (header.bfType != MAGIC_NUMBER_BMP) 
			throw std::string("ouverture format BMP impossible ..."); 
		else {
			f.read((char*)&infoHeader,sizeof(BITMAPINFOHEADER));
			if (infoHeader.biCompression > 0) 
				throw std::string("Format compresse non supporté...");
			else {
				if (infoHeader.biBitCount == 24) {
					this->m_iHauteur = infoHeader.biHeight;
					this->m_iLargeur = infoHeader.biWidth;
					this->m_sNom.assign(name.begin(),name.end()-4);
					this->m_pucData = new unsigned char[infoHeader.biHeight*infoHeader.biWidth*3];
					this->m_ppucPixel = new unsigned char*[infoHeader.biHeight*infoHeader.biWidth];
					for (int i=0;i<infoHeader.biHeight*infoHeader.biWidth;i++)
						this->m_ppucPixel[i] = &this->m_pucData[3*i];

					// gérer multiple de 32 bits via zéros éventuels ignorés
					int complement = (((this->m_iLargeur*3-1)/4) + 1)*4 - this->m_iLargeur*3;

					for (int i= m_iHauteur-1; i >= 0; i--) {
						for (int j=0;j<m_iLargeur;j++) {
							f.read((char*)&this->m_ppucPixel[i*m_iLargeur+j][2],sizeof(char));
							f.read((char*)&this->m_ppucPixel[i*m_iLargeur+j][1],sizeof(char)); 
							f.read((char*)&this->m_ppucPixel[i*m_iLargeur+j][0],sizeof(char));
					}	

					char inutile;
					for (int k=0; k< complement; k++)
						f.read((char*)&inutile,sizeof(char));
					}
				}
				else {
					// cas d'une image en niveaux de gris
					this->m_iHauteur = infoHeader.biHeight;
					this->m_iLargeur = infoHeader.biWidth;
					this->m_sNom.assign(name.begin(),name.end()-4);
					this->m_pucData = new unsigned char[infoHeader.biHeight*infoHeader.biWidth*3];
					this->m_ppucPixel = new unsigned char*[infoHeader.biHeight*infoHeader.biWidth];
					for (int i=0;i<infoHeader.biHeight*infoHeader.biWidth;i++)
						this->m_ppucPixel[i] = &this->m_pucData[3*i];

					// lecture palette
					unsigned char* palette=NULL;
					palette = new unsigned char[256*4];
					for (int indice=0;indice<4*256;indice++) 
						f.read((char*)&palette[indice],sizeof(char));
					
					// gérer multiple de 32 bits via zéros éventuels ignorés
					int complement = (((this->m_iLargeur-1)/4) + 1)*4 - this->m_iLargeur;
					// passage du gris vers la couleur par duplication des valeurs
					for (int i= m_iHauteur-1; i >= 0; i--) {
						for (int j=0;j<m_iLargeur;j++) {
							unsigned char temp;
							f.read((char*)&temp,sizeof(char));
							this->m_ppucPixel[i*m_iLargeur+j][0]=palette[4*temp+2];
							this->m_ppucPixel[i*m_iLargeur+j][1]=palette[4*temp+1];
							this->m_ppucPixel[i*m_iLargeur+j][2]=palette[4*temp];
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
		throw std::string("Ouverture impossible !");
}

CImageCouleur::CImageCouleur(const CImageCouleur& im) {

	this->m_iHauteur = im.lireHauteur();
	this->m_iLargeur = im.lireLargeur();
	this->m_sNom     = im.lireNom();
	this->m_pucData = NULL;
	this->m_ppucPixel = NULL;

	if (im.m_pucData != NULL) {
		this->m_pucData = new unsigned char[im.lireHauteur()*im.lireLargeur()*3];
		this->m_ppucPixel = new unsigned char*[im.lireHauteur()*im.lireLargeur()];
		for (int i=0;i<im.lireHauteur()*im.lireLargeur();i++)
			this->m_ppucPixel[i] = &this->m_pucData[3*i];
		
		memcpy(this->m_pucData,im.m_pucData,im.lireNbPixels()*3);
		}
}

CImageCouleur::~CImageCouleur() {

	if (this->m_ppucPixel) {
		delete[] this->m_ppucPixel;
		this->m_ppucPixel = NULL;
	}
	if (this->m_pucData) {
		delete[] this->m_pucData;
		this->m_pucData = NULL;
	}
}

void CImageCouleur::sauvegarde(const std::string& fixe) {

	BITMAPFILEHEADER header;
	BITMAPINFOHEADER infoHeader;

	if (this->m_ppucPixel) {
		std::string nomFichier = "../Res/";
		if (fixe.compare("") == 0)
			nomFichier += this->lireNom() + ".bmp"; // force sauvegarde dans répertoire Res (doit exister)
		else
			nomFichier += fixe;
		std::ofstream f(nomFichier.c_str(),std::ios::binary);
		if (f.is_open()) {
			int complement = (((this->m_iLargeur*3-1)/4) + 1)*4 - this->m_iLargeur*3;

			header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); 
			header.bfSize = header.bfOffBits + (complement+lireLargeur())*lireHauteur()*sizeof(char);
			header.bfType = MAGIC_NUMBER_BMP;
			header.bfReserved1 = 0;
			header.bfReserved2 = 0;
			f.write((char*)&header,sizeof(BITMAPFILEHEADER));
			
			infoHeader.biHeight = this->lireHauteur();
			infoHeader.biWidth = this->lireLargeur();
			infoHeader.biCompression = 0;
			infoHeader.biClrUsed = 0;
			infoHeader.biBitCount = 24;
			infoHeader.biSizeImage = 0; //pas de compression;
			infoHeader.biClrUsed = 0;
			infoHeader.biClrImportant = 0;
			infoHeader.biHeight = this->lireHauteur();
			infoHeader.biWidth = this->lireLargeur();
			infoHeader.biPlanes = 1;
			infoHeader.biSize = sizeof(infoHeader);
			infoHeader.biSizeImage = this->lireNbPixels();
			f.write((char*)&infoHeader,sizeof(BITMAPINFOHEADER));

			for (int i= this->lireHauteur()-1; i >= 0; i--) {
				for (int j=0;j<this->lireLargeur();j++) {
					f.write((char*)&this->m_ppucPixel[i*this->lireLargeur()+j][2],sizeof(char));
					f.write((char*)&this->m_ppucPixel[i*this->lireLargeur()+j][1],sizeof(char)); 
					f.write((char*)&this->m_ppucPixel[i*this->lireLargeur()+j][0],sizeof(char));
				}	
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

CImageCouleur& CImageCouleur::operator=(const CImageCouleur& im) {

	if (&im == this)
		return *this;

	this->m_iHauteur = im.lireHauteur();
	this->m_iLargeur = im.lireLargeur();
	this->m_sNom     = im.lireNom();

	if (this->m_pucData) 
		delete[] this->m_pucData;
	if (this->m_ppucPixel) 
		delete[] this->m_ppucPixel;

	this->m_pucData = new unsigned char[im.lireHauteur()*im.lireLargeur()*3];
	this->m_ppucPixel = new unsigned char*[im.lireHauteur()*im.lireLargeur()];
	for (int i=0;i<im.lireHauteur()*im.lireLargeur();i++)
		this->m_ppucPixel[i] = &this->m_pucData[3*i];

	memcpy(this->m_pucData,im.m_pucData,im.lireNbPixels()*3);

return *this;
}

// histogramme 

std::vector<std::vector<unsigned long>> CImageCouleur::histogramme(bool enregistrementCSV, int pas) {

	std::vector<std::vector<unsigned long>> h;

	h.resize(3); // 3 plans 
	for (int plan=0;plan<3;plan++) 
		h[plan].resize(256/pas,0);

	for (int i=0;i<this->lireNbPixels();i++) { 
		h[0][this->operator()(i)[0]/pas] += 1L;
		h[1][this->operator()(i)[1]/pas] += 1L;
		h[2][this->operator()(i)[2]/pas] += 1L;
	}
	if (enregistrementCSV) {
		std::string fichier = "../Res/" + this->lireNom() + ".csv";
		std::ofstream f (fichier.c_str());

		if (!f.is_open())
			std::cout << "Impossible d'ouvrir le fichier en ecriture !" << std::endl;
		else {
			for (int i=0;i<(int)h[0].size();i++)
				f << h[0][i] << " ; " << h[1][i] << " ; " << h[2][i] << " ; " << std::endl;
		}
		f.close();
	}

	return h;
}


// gestion des plans
CImageNdg CImageCouleur::plan(int choix, float poidsRouge, float poidsVert, float poidsBleu) { // 0 -> R ou H, 1 -> V ou S, 2 -> B ou V et 3 -> luminance d'où les poids fct de l'illuminant
	
	CImageNdg out(this->lireHauteur(),this->lireLargeur());
	std::stringstream convert;
	convert << choix;
	out.ecrireNom(this->lireNom() + convert.str());
	out.choixPalette("grise"); // par défaut
	out.ecrireBinaire(false);

	switch(choix) {
	case 0 :
		for (int i=0;i<this->lireNbPixels();i++)
			out(i) = this->operator()(i)[0];
		break;
	case 1 :
		for (int i=0;i<this->lireNbPixels();i++)
			out(i) = this->operator()(i)[1];
		break;
	case 2 : 
		for (int i=0;i<this->lireNbPixels();i++)
			out(i) = this->operator()(i)[2];
		break;
	case 3: 
		for (int i=0;i<this->lireNbPixels();i++)
			out(i) = (unsigned char)(poidsRouge*(float)this->operator()(i)[0] + poidsVert*(float)this->operator()(i)[1] + poidsBleu*(float)this->operator()(i)[2]);
	}
	return out;
}

CImageCouleur& CImageCouleur::miseAJourPlan(int choix, const CImageNdg& plan) {

	for (int i=0;i<this->lireNbPixels();i++)
		this->operator()(i)[choix] = plan(i);

		return *this;
}


// conversion espace HSV sur [0-255] pour chaque plan

CImageCouleur CImageCouleur::conversion(const std::string& methode) const {

	CImageCouleur out(this->lireHauteur(),this->lireLargeur(),0,0,0);
	out.ecrireNom(this->lireNom() + "HSV");

	if (methode.compare("HSV") == 0) {
		unsigned char rgb_min, rgb_max;	

		for (int i=0;i<this->lireNbPixels();i++) 
		{
			rgb_min = MIN3(this->operator()(i)[0], this->operator()(i)[1], this->operator()(i)[2]);
			rgb_max = MAX3(this->operator()(i)[0], this->operator()(i)[1], this->operator()(i)[2]);

			if (rgb_max != rgb_min)
			{
				if (rgb_max == this->operator()(i)[0]) {
					out(i)[0] = 0 + 43*(this->operator()(i)[1] - this->operator()(i)[2])/(rgb_max - rgb_min);
				}
				else 
					if (rgb_max == this->operator()(i)[1]) {
						out(i)[0] = 85 + 43*(this->operator()(i)[2] - this->operator()(i)[0])/(rgb_max - rgb_min);
					}
					else  {
						out(i)[0] = 171 + 43*(this->operator()(i)[0] - this->operator()(i)[1])/(rgb_max - rgb_min);
					}
			}
			if (rgb_max != 0)
				out(i)[1] = rgb_max-rgb_min;
			out(i)[2] = rgb_max;
		}
	}
	return out;
}

CImageCouleur CImageCouleur::transformation(const std::string& methode, int vMin, int vMax) {

	CImageCouleur out(this->lireHauteur(),this->lireLargeur());
	out.m_sNom     = this->lireNom()+"T";

	if (methode.compare("expansion") == 0) {
		CImageNdg pR = this->plan(0);
		CImageNdg pG = this->plan(1);
		CImageNdg pB = this->plan(2);

		pR = pR.transformation("expansion",vMin,vMax);
		pG = pG.transformation("expansion",vMin,vMax);
		pB = pB.transformation("expansion",vMin,vMax);

		out.miseAJourPlan(0,pR);
		out.miseAJourPlan(1,pG);
		out.miseAJourPlan(2,pB);
		}
		else
			if (methode.compare("egalisation") == 0) {
				CImageNdg pR = this->plan(0);
				CImageNdg pG = this->plan(1);
				CImageNdg pB = this->plan(2);

				pR = pR.transformation("egalisation");
				pG = pG.transformation("egalisation");
				pB = pB.transformation("egalisation");

				out.miseAJourPlan(0,pR);
				out.miseAJourPlan(1,pG);
				out.miseAJourPlan(2,pB);
			}

	return out;
}

std::vector<MOMENTS> CImageCouleur::signatures()
{
	std::vector<MOMENTS> mts;

	mts.resize(3);

	for (int plan = 0; plan < 3; plan++)
	{
		CImageNdg p = this->plan(plan);

		std::vector<unsigned long> hist;
		hist = p.histogramme();

		mts.at(plan) = p.signatures(hist);
	}

	return mts;
}