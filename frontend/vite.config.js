import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [react()],
  server: {
    proxy: {
      // '/api' ile baslayan her istegi yakala
      '/api': {
        target: 'http://127.0.0.1:8085', // C++ Sunucusuna gonder
        changeOrigin: true,
        secure: false,
        rewrite: (path) => path.replace(/^\/api/, '') // '/api' kismini sil, sadece '/login' gitsin
      }
    }
  }
})