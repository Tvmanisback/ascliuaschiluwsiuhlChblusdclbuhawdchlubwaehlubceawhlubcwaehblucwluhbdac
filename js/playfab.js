// Minimal PlayFab client helpers for browser
import { CONFIG } from './config.js';

const PLAYFAB_API = `https://${CONFIG.playFabTitleId}.playfabapi.com`;

export async function loginWithCustomId(customId) {
  const res = await fetch(`${PLAYFAB_API}/Client/LoginWithCustomID`, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({
      TitleId: CONFIG.playFabTitleId,
      CustomId: customId,
      CreateAccount: true
    })
  });
  const data = await res.json();
  if (data.errorMessage) throw new Error(data.errorMessage);
  return data.data; // contains SessionTicket, PlayFabId, etc.
}

export async function getPhotonAuthenticationToken(sessionTicket, photonAppId) {
  const res = await fetch(`${PLAYFAB_API}/Client/GetPhotonAuthenticationToken`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
      'X-Authorization': sessionTicket
    },
    body: JSON.stringify({
      PhotonApplicationId: photonAppId
    })
  });
  const data = await res.json();
  if (data.errorMessage) throw new Error(data.errorMessage);
  return data.data.PhotonCustomAuthenticationToken;
}
