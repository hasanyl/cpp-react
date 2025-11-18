import { useEffect, useState } from 'react';
import { useNavigate } from 'react-router-dom';

function Home() {
  const [kullanici, setKullanici] = useState(null);
  const navigate = useNavigate();

  useEffect(() => {
    // Tarayıcı hafızasından kullanıcı adını kontrol et
    const kayitliKullanici = localStorage.getItem("aktifKullanici");
    if (kayitliKullanici) {
      setKullanici(kayitliKullanici);
    }
  }, []);

  const cikisYap = () => {
    localStorage.removeItem("aktifKullanici");
    setKullanici(null);
  };

  return (
    <div style={{ textAlign: 'center', marginTop: '50px' }}>
      <h1>Ana Sayfa</h1>
      
      {kullanici ? (
        <div style={{ border: '2px solid green', padding: '20px', display: 'inline-block', borderRadius: '10px' }}>
          <h2 style={{ color: 'green' }}>Hoşgeldin, {kullanici}! 👋</h2>
          <p>Başarıyla giriş yaptın.</p>
          <button onClick={cikisYap} style={{ backgroundColor: '#ff4444' }}>Çıkış Yap</button>
        </div>
      ) : (
        <div style={{ border: '1px solid #ccc', padding: '30px', display: 'inline-block', borderRadius: '10px' }}>
          <h3>Lütfen bir işlem seçiniz:</h3>
          <div style={{ display: 'flex', gap: '20px', justifyContent: 'center' }}>
            <button onClick={() => navigate("/login")}>Giriş Yap</button>
            <button onClick={() => navigate("/register")}>Kayıt Ol</button>
          </div>
        </div>
      )}
    </div>
  );
}

export default Home;