"use client";
import { useRouter } from "next/navigation";


export default function Welcome() {
const router = useRouter();
  return (
    <div style={styles.container}>
      <h1 style={styles.title}>Smart Helmet System</h1>
      <p style={styles.subtitle}>Realtime Safety Monitoring</p>
      <button  className="bg-gray-900 p-3 cursor-pointer rounded-xl hover:bg-gray-800"onClick={(e)=>{
        router.push("/dashboard")
      }}> Sensor Dashboard</button>
    </div>
  );
}

const styles: any = {
  container: {
    display: "flex",
    flexDirection: "column",
    justifyContent: "center",
    alignItems: "center",
    height: "100vh",
    background: "linear-gradient(135deg, #090979, #00d4ff)",
    color: "white",
    textAlign: "center",
  },
  title: {
    fontSize: 48,
    marginBottom: 20,
    fontWeight: "bold",
  },
  subtitle: {
    fontSize: 20,
    marginBottom: 40,
    opacity: 0.8,
  },
  loader: {
    width: 60,
    height: 60,
    border: "6px solid rgba(255,255,255,0.3)",
    borderTop: "6px solid #fff",
    borderRadius: "50%",
    animation: "spin 1s linear infinite",
  },
};

