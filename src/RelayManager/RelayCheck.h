// #ifndef RELAYCHECK_H
// #define RELAYCHECK_H

// #include "../FirestoreManager/FirestoreManager.h"
// // #include "JsonParser.h"
// #include "RelayManager.h"
// #include "../info.h"

// class RelayCheck {
// public:
//     RelayCheck(FirestoreManager& firestoreManager, RelayManager& relayManager);
//     void updateRelays(); // Firestore'dan okunan verilere göre röleleri günceller

// private:
//     FirestoreManager& _firestoreManager;
//     RelayManager& _relayManager;
// };

// #endif


#ifndef RELAY_CHECK_H
#define RELAY_CHECK_H

#include <Arduino.h>
#include "RelayManager.h"
#include "Project_info_panel.h"

class FirestoreManager;  // Forward Declaration
class RelayManager;      // Forward Declaration

class RelayCheck {
public:
    RelayCheck(FirestoreManager& firestoreManager, RelayManager& relayManager);

    void updateRelays();

private:
    FirestoreManager& _firestoreManager;
    RelayManager& _relayManager;
};

#endif
