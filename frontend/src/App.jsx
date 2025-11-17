import { useState } from 'react'
import './App.css'

function App() {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [message, setMessage] = useState("");

  const handleLogin = async (e) => {
    e.preventDefault(); // Sayfanın yenilenmesini engelle
    setMessage("Sunucuya baglaniliyor...");

    try {
      // 1. Istegi hazirla (POST methodu ile)
      const response = await fetch("/api/login", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({ 
          kullanici: username, 
          sifre: password 
        }),
      });

      // 2. Cevabi oku
      const data = await response.json();
      setMessage(data.cevap); // Backend'den gelen mesaji ekrana yaz

    } catch (error) {
      console.error("Hata:", error);
      setMessage("Sunucuya ulaşılamadı!");
    }
  };

  return (
    <div style={{ 
      padding: '50px',
      textAlign: 'center',
      backgroundColor : '#FFF', 
      color : '#000', 
      border : '2px solid #000', 
      borderRadius : '10px',
      boxShadow : '0px 4px 8px white' }}>
      <h2>Giriş Sistemi (C++ Backend)</h2>
      
      <form onSubmit={handleLogin} style={{ display: 'inline-block', textAlign: 'left' }}>
        <div style={{ marginBottom: '10px' }}>
          <label>Kullanıcı Adı:</label><br/>
          <input 
            type="text" 
            value={username}
            onChange={(e) => setUsername(e.target.value)}
          />
        </div>
        
        <div style={{ marginBottom: '10px' }}>
          <label>Şifre:</label><br/>
          <input 
            type="password" 
            value={password}
            onChange={(e) => setPassword(e.target.value)}
          />
        </div>

        <button type="submit">Giriş Yap</button>
      </form>

      <h3 style={{ color: 'blue', marginTop: '20px' }}>{message}</h3>
    </div>
  )
}

export default App