#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>
#include <cmath>
#include <vector>
#include <ctime>
#include <stack>

#include "libTraitement.h"

ClibTraitement::ClibTraitement() {

	this->nbDataImg = 0;
	this->dataFromImg.clear();
	this->imgPt = NULL;
}

// Méthode de traitement par défaut -> constructeur par défaut via le C#
ClibTraitement::ClibTraitement(int nbChamps, byte* data, int stride, int nbLig, int nbCol){

	// affectation des signatures (dataFromImg) et taille (nbChamps)
	this->nbDataImg = nbChamps;
	this->dataFromImg.resize(nbChamps);

	this->imgPt = new CImageCouleur(nbLig, nbCol);
	CImageCouleur out(nbLig, nbCol);

	// byte* data est la donnée des images

	byte* pixPtr = (byte*)data;

	for (int y = 0; y < nbLig; y++)
	{
		for (int x = 0; x < nbCol; x++)	//rvb
		{
			this->imgPt->operator()(y, x)[0] = pixPtr[3 * x + 2];
			this->imgPt->operator()(y, x)[1] = pixPtr[3 * x + 1];
			this->imgPt->operator()(y, x)[2] = pixPtr[3 * x ];
		}
		pixPtr += stride; // largeur une seule ligne gestion multiple 32 bits
	}
	
	//CImageNdg imgsauv;
	//imgsauv = this->imgPt->plan();
	//imgsauv.sauvegarde("test")



	/*********************************************************************************************************************

	DEBUT DE L'ALGORITHME DE TRAITEMENT

	**********************************************************************************************************************/

	CImageNdg seuil;

	int seuilBas = 128;
	int seuilHaut = 255;

	seuil = this->imgPt->plan().seuillage("automatique",seuilBas,seuilHaut);

	this->dataFromImg.at(0) = seuilBas;

	for (int i = 0; i < seuil.lireNbPixels(); i++)
	{
		out(i)[0] = (unsigned char)(255*(int)seuil(i));
		out(i)[1] = 0;
		out(i)[2] = 0;
	}
		
	/*********************************************************************************************************************

	FIN DU TRAITEMENT,
	AFFICHAGE RETOUR VERS LE C#

	**********************************************************************************************************************/

	pixPtr = (byte*)data;
	for (int y = 0; y < nbLig; y++)
	{
		for (int x = 0; x < nbCol; x++)
		{
			pixPtr[3 * x + 2] = out(y, x)[0];
			pixPtr[3 * x + 1] = out(y, x)[1];
			pixPtr[3 * x] = out(y, x)[2];
		}
		pixPtr += stride; // largeur une seule ligne gestion multiple 32 bits
	}
}


ClibTraitement::~ClibTraitement() {
	
	if (imgPt)
		(*this->imgPt).~CImageCouleur(); 
	this->dataFromImg.clear();
}

void ClibTraitement::Traitement2(int nbChamps, byte * data, int stride, int nbLig, int nbCol, double parametres[10], int nbChamps_p, byte * data_p, int stride_p, int nbLig_p, int nbCol_p)
{
	// --------------------------- Construction de l'image ClibTraitement ----------------------------------
	// affectation des signatures (dataFromImg) et taille (nbChamps)
	this->nbDataImg = nbChamps;
	this->dataFromImg.resize(nbChamps);

	this->imgPt = new CImageCouleur(nbLig, nbCol);
	CImageCouleur out(nbLig, nbCol);
	// byte* data est la donnée des images
	byte* pixPtr = (byte*)data;

	for (int y = 0; y < nbLig; y++)
	{
		for (int x = 0; x < nbCol; x++)	//rvb
		{
			this->imgPt->operator()(y, x)[0] = pixPtr[3 * x + 2];
			this->imgPt->operator()(y, x)[1] = pixPtr[3 * x + 1];
			this->imgPt->operator()(y, x)[2] = pixPtr[3 * x];
		}
		pixPtr += stride; // largeur une seule ligne gestion multiple 32 bits
	}

	// Création d'une image pour la piece envoyee
	/*
	ClibTraitement piece;
	piece.nbDataImg = nbChamps_p;
	piece.dataFromImg.resize(nbChamps_p);
	piece.imgPt = new CImageCouleur(nbLig_p, nbCol_p);
	byte* pixPtr_p = (byte*)data_p;
	for (int y = 0; y < nbLig_p; y++)
	{
		for (int x = 0; x < nbCol_p; x++)
		{
			piece.imgPt->operator()(y, x)[0] = pixPtr_p[3 * x + 2];
			piece.imgPt->operator()(y, x)[1] = pixPtr_p[3 * x + 1];
			piece.imgPt->operator()(y, x)[2] = pixPtr_p[3 * x];
		}
		pixPtr_p += stride_p;
	}
	*/
	
	CImageCouleur piece2(nbLig_p, nbCol_p);
	byte* pixPtr_p2 = (byte*)data_p;
	for (int y = 0; y < nbLig_p; y++)
	{
		for (int x = 0; x < nbCol_p; x++)
		{
			piece2(y, x)[0] = pixPtr_p2[3 * x + 2];
			piece2(y, x)[1] = pixPtr_p2[3 * x + 1];
			piece2(y, x)[2] = pixPtr_p2[3 * x];
		}
		pixPtr_p2 += stride_p;
	}


	/*********************************************************************************************************************

	DEBUT DE L'ALGORITHME DE TRAITEMENT

	**********************************************************************************************************************/

	// Traitement sur image niveaux de gris qui était présent par le passé

	//CImageNdg moyen;
	//moyen = this->imgPt->plan().filtrage("moyennage", (int)parametres[0], (int)parametres[1]);
	//this->dataFromImg.at(0) = parametres[0];
	//for (int i = 0; i < moyen.lireNbPixels(); i++)
	//{
	//	out(i)[0] = (unsigned char)(255 * (int)moyen(i));
	//	out(i)[1] = (unsigned char)(255 * (int)moyen(i));
	//	out(i)[2] = (unsigned char)(255 * (int)moyen(i));
	//}


	// Appel à la fonction detection piece qui a partir de detection_ref et piece2 renvoie une image du puzzle avec un cadre rouge au niveau de la détection
	CImageCouleur detection_ref;
	detection_ref = this->imgPt->detection_piece(piece2);

	// Copie de l'image résultante dans out pour renvoyer au C#
	for (int i = 0; i < detection_ref.lireNbPixels(); i++)
	{
		out(i)[0] = detection_ref(i)[0];
		out(i)[1] = detection_ref(i)[1];
		out(i)[2] = detection_ref(i)[2];
	}

	/*********************************************************************************************************************

	FIN DU TRAITEMENT,
	AFFICHAGE RETOUR VERS LE C#

	**********************************************************************************************************************/

	pixPtr = (byte*)data;
	for (int y = 0; y < nbLig; y++)
	{
		for (int x = 0; x < nbCol; x++)
		{
			pixPtr[3 * x + 2] = out(y, x)[0];
			pixPtr[3 * x + 1] = out(y, x)[1];
			pixPtr[3 * x] = out(y, x)[2];
		}
		pixPtr += stride; // largeur une seule ligne gestion multiple 32 bits
	}
}
