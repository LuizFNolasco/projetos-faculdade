import Link from "next/link";

import { UsuarioForm } from "@/components/usuarios/usuario-form";

export default function Cadastro() {
  return (
    <>


      <div className="mb-10 text-center">
        <h2 className="text-3xl font-bold tracking-wider text-white">
          CRIAR CONTA
        </h2>
        <p className="mt-2 text-sm font-medium text-[#04D939]">
          Junte-se ao work &amp; rest
        </p>
      </div>

      <UsuarioForm mode="create" variant="cadastro" redirectTo="/login" />
    </>
  );
}
