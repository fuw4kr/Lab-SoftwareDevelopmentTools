/**
 * @file Menu.h
 * @brief Declares the user interface function for the social network application.
 * @details Provides the main console-based menu used to interact with the SocialNetwork class.
 *
 * @see runMenu()
 * @see SocialNetwork
 *
 * @date 04.11.2025
 * @version 1.0
 * @author Kristina Zakharchenko
 */

#ifndef MENU_H
#define MENU_H

#include "SocialNetwork.h"

 /**
  * @brief Launches the interactive console menu for the social network.
  * @details Allows users to perform various actions such as adding users, friendships,
  * sending messages, creating posts, exporting graphs, and more.
  *
  * @param net Reference to a SocialNetwork instance.
  * @example
  * SocialNetwork network;
  * runMenu(network);
  */
void runMenu(SocialNetwork& net);

#endif // MENU_H
