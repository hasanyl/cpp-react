import { useState } from 'react';
import { useNavigate } from 'react-router-dom';

function Register() {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [mesaj, setMesaj] = useState("");
  const navigate = useNavigate();

  const handleRegister = async (e) => {
    e.preventDefault();
    
    try {
      const response = await fetch("/api/register", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ kullanici: username, sifre: password }),
      });

      const data = await response.json();

      if (response.ok) {
        alert("Kayıt Başarılı! Giriş sayfasına yönlendiriliyorsunuz.");
        navigate("/login");
      } else {
        setMesaj(data.cevap);
      }

    } catch (error) {
      setMesaj("Sunucu hatası.");
    }
  };

  return (
    <div style={{ textAlign: 'center', marginTop: '50px' }}>
      <h2>Kayıt Ol</h2>
      <form onSubmit={handleRegister} style={{ display: 'inline-block', textAlign: 'left' }}>
        <div>
          <label>Kullanıcı Adı Seç:</label><br/>
          <input type="text" onChange={(e) => setUsername(e.target.value)} required />
        </div>
        <div style={{ marginTop: '10px' }}>
          <label>Şifre Belirle:</label><br/>
          <input type="password" onChange={(e) => setPassword(e.target.value)} required />
        </div>
        <button type="submit" style={{ marginTop: '15px', width: '100%' }}>Kayıt Ol</button>
      </form>
      
      {mesaj && <p style={{ color: 'red' }}>{mesaj}</p>}
      
      <p>Zaten hesabın var mı? <span style={{color:'blue', cursor:'pointer'}} onClick={() => navigate("/login")}>Giriş Yap</span></p>
    </div>
  );
}

export default Register;