import { useState } from 'react';
import { useNavigate } from 'react-router-dom';

function Login() {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [hata, setHata] = useState("");
  const navigate = useNavigate();

  const handleLogin = async (e) => {
    e.preventDefault();
    setHata("");

    try {
      const response = await fetch("/api/login", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ kullanici: username, sifre: password }),
      });

      const data = await response.json();

      if (response.ok) {
        localStorage.setItem("aktifKullanici", username);
        navigate("/"); 
      } else {
        setHata(data.cevap);
      }
    } catch (error) {
      setHata("Sunucuya bağlanılamadı!");
    }
  };

  return (
    <div style={{ textAlign: 'center', marginTop: '50px' }}>
      <h2>Giriş Yap</h2>
      <form onSubmit={handleLogin} style={{ display: 'inline-block', textAlign: 'left' }}>
        <div>
          <label>Kullanıcı Adı:</label><br/>
          <input type="text" onChange={(e) => setUsername(e.target.value)} required />
        </div>
        <div style={{ marginTop: '10px' }}>
          <label>Şifre:</label><br/>
          <input type="password" onChange={(e) => setPassword(e.target.value)} required />
        </div>
        <button type="submit" style={{ marginTop: '15px', width: '100%' }}>Giriş</button>
      </form>
      
      {hata && <p style={{ color: 'red' }}>{hata}</p>}
      
      <p>Hesabın yok mu? <span style={{color:'blue', cursor:'pointer'}} onClick={() => navigate("/register")}>Kayıt Ol</span></p>
    </div>
  );
}

export default Login;