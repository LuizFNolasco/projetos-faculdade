import Link from "next/link";

import { LoginForm } from "@/components/usuarios/login-form";

export default function Login() {
  return (
    <>


      <div className="relative z-10 mb-10 text-center">
        <h2 className="text-3xl font-bold tracking-wider text-white">
          BEM-VINDO
        </h2>
        <p className="mt-2 text-sm font-medium text-[#04D939]">
          Acesse sua conta
        </p>
      </div>

      <LoginForm />
    </>
  );
}
