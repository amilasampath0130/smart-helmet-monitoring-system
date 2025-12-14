import { initializeApp } from "firebase/app";
import { getDatabase } from "firebase/database";

const firebaseConfig = {
  apiKey: "AIzaSyBSjD-YEVOw0uyE-ae5DYgaIr2uZaKP6AU",
  authDomain: "smart-helmet-system-c95f1.firebaseapp.com",
  databaseURL: "https://smart-helmet-system-c95f1-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "smart-helmet-system-c95f1",
  storageBucket: "smart-helmet-system-c95f1.firebasestorage.app",
  messagingSenderId: "950867416658",
  appId: "1:950867416658:web:769f5293f409371a82f18b"
};

const app = initializeApp(firebaseConfig);

export const db = getDatabase(app);
