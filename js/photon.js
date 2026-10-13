// Photon Realtime + Voice (Chat App) connection helpers
// Uses PlayFab custom authentication

import { CONFIG } from './config.js';
import { getPhotonAuthenticationToken } from './playfab.js';

/**
 * After PlayFab login you must:
 * 1. Call GetPhotonAuthenticationToken for Realtime App ID
 * 2. Call GetPhotonAuthenticationToken for Chat App ID (Voice)
 * 3. Connect to Photon using Custom Authentication with username=PlayFabId + token
 *
 * This file shows the data shape you need.
 * Full Photon JS SDK must be loaded from Photon CDN or your own build.
 */

export async function preparePhotonAuth(sessionTicket, playFabId) {
  const realtimeToken = await getPhotonAuthenticationToken(
    sessionTicket,
    CONFIG.photonRealtimeAppId
  );

  const chatToken = await getPhotonAuthenticationToken(
    sessionTicket,
    CONFIG.photonChatAppId   // listed as Chat App
  );

  return {
    playFabId,
    realtime: {
      appId: CONFIG.photonRealtimeAppId,
      token: realtimeToken,
      region: CONFIG.photonRegion
    },
    chat: {                 // Voice listed as Chat App
      appId: CONFIG.photonChatAppId,
      token: chatToken,
      region: CONFIG.photonRegion
    }
  };
}

/*
  Example usage with Photon JS SDK (you must include the official Photon scripts):

  const auth = await preparePhotonAuth(sessionTicket, playFabId);

  // Realtime
  client.AuthValues = new Photon.LoadBalancing.AuthenticationValues();
  client.AuthValues.AuthType = Photon.LoadBalancing.Constants.CustomAuthenticationType.Custom;
  client.AuthValues.AddAuthParameter("username", auth.playFabId);
  client.AuthValues.AddAuthParameter("token", auth.realtime.token);
  client.connectToRegionMaster(auth.realtime.region);

  // Voice / Chat App (same pattern)
  voiceClient.AuthValues = ...
  voiceClient.AuthValues.AddAuthParameter("username", auth.playFabId);
  voiceClient.AuthValues.AddAuthParameter("token", auth.chat.token);
*/
