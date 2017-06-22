#pragma once
#include "minko/Minko.hpp"
#include "minko/MinkoSDL.hpp"
#include "minko/MinkoHtmlOverlay.hpp"
#include "minko/MinkoBullet.hpp"
#include "minko/MinkoASSIMP.hpp"
#include "minko/MinkoJPEG.hpp"
#include "minko/MinkoPNG.hpp"

#include "Python.h"
#include "structmember.h"

using namespace minko;
using namespace minko::component;
using namespace minko::component::bullet;

// Constantes
CONST std::string TITULO_VENTANA = "E.N.N.I.";

Canvas::Ptr canvas;
SceneManager::Ptr sceneManager;