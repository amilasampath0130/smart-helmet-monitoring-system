"use client";

import { useEffect, useState } from "react";
import { ref, onValue } from "firebase/database";
import { db } from "@/lib/firebase";
import {
  Thermometer,
  Droplets,
  AlertTriangle,
} from "lucide-react";
import Image from "next/image";

const image = require("../app/image/WhatsApp Image 2025-12-08 at 8.57.58 PM.jpeg");

export default function Dashboard() {
  const [temperature, setTemperature] = useState<number>(0);
  const [humidity, setHumidity] = useState<number>(0);
  const [gas, setGas] = useState<number>(0);

  const [lat, setLat] = useState<number | null>(null);
  const [lng, setLng] = useState<number | null>(null);
  const [gpsTime, setGpsTime] = useState<string>("");

  useEffect(() => {
    const sensorRef = ref(db, "Sensor");

    const unsubscribe = onValue(sensorRef, (snapshot) => {
      const data = snapshot.val();
      if (data) {
        setTemperature(data.Temperature ?? 0);
        setHumidity(data.Humidity ?? 0);
        setGas(data.Gas ?? 0);

        if (data.GPS) {
          setLat(data.GPS.Latitude ?? null);
          setLng(data.GPS.Longitude ?? null);
          setGpsTime(data.GPS.Time ?? "");
        }
      }
    });

    return () => unsubscribe();
  }, []);

  const gasStatus = gas < 500 ? "SAFE" : gas < 1000 ? "WARNING" : "DANGER";

  return (
    <div style={styles.bg}>
      <div className="flex justify-center items-center gap-6 my-6">
        <div style={{ textAlign: "center" }}>
          <h1 style={styles.title}>Smart helmet monitoring System</h1>
          <h2 style={styles.sub_title}>ETEC 3173 - INSTRUMENTATION</h2>
        </div>
        <div>
          <Image src={image} width={120} alt="Helmet Image" />
        </div>
      </div>

      <div style={styles.grid}>
        <div style={styles.card}>
          <Thermometer size={40} />
          <h2 className=" text-2xl">Temperature</h2>
          <p style={styles.value}>{temperature} °C</p>
        </div>
        <div style={styles.card}>
          <Droplets size={40} />
          <h2 >Humidity</h2>
          <p style={styles.value}>{humidity} %</p>
        </div>
        <div style={styles.card}>
          <AlertTriangle size={40} />
          <div>
            <h2 >Gas Level</h2>
            <p style={styles.value}>{gas}</p>
            <p style={styles.status}>{gasStatus}</p>
          </div>
        </div>
        <div style={styles.card}>
          <h2>Location</h2>

          {lat && lng ? (
            <>
              <iframe
                width="100%"
                height="250"
                loading="lazy"
                style={{
                  borderRadius: 20,
                  marginTop: 15,
                  border: "2px solid white",
                }}
                src={`https://www.google.com/maps?q=${lat},${lng}&z=15&output=embed`}
              ></iframe>
              <a
                href={`https://www.google.com/maps?q=${lat},${lng}`}
                target="_blank"
                style={{
                  marginTop: 15,
                  display: "inline-block",
                  color: "black",
                  textDecoration: "underline",
                  fontWeight: "bold",
                }}
              >
                Open in Google Maps
              </a>
            </>
          ) : (
            <p>No GPS Data</p>
          )}
        </div>
      </div>
    </div>
  );
}

const styles: any = {
  bg: {
    minHeight: "100vh",
    background: "oklch(62.3% 0.214 259.815)",
    color: "white",
    padding: 40,
  },
  title: {
    textAlign: "center",
    fontSize: 40,
    marginBottom: 20,
  },
  sub_title: {
    textAlign: "center",
    fontSize: 35,
    marginBottom: 20,
  },
  grid: {
    display: "grid",
    gridTemplateColumns: "repeat(auto-fit, minmax(250px, 1fr))",
    gap: 30,
  },
  card: {
    background: "rgba(255,255,255,0.12)",
    borderRadius: 20,
    padding: 30,
    textAlign: "center",
    backdropFilter: "blur(10px)",
    boxShadow: "0 0 20px rgba(0,0,0,0.4)",
    transition: "0.3s",
  },
  value: {
    fontSize: 60,
    fontWeight: "bold",
    marginTop: 10,
  },
  status: {
    marginTop: 10,
    fontSize: 25,
    letterSpacing: 2,
  },
};
