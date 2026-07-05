import { NextResponse } from "next/server";
import { getSessionUsuario } from "@/lib/auth/session";
import { prisma } from "@/lib/db";

export async function DELETE(
  req: Request,
  { params }: { params: { id: string } }
) {
  try {
    const session = await getSessionUsuario();
    
    if (!session) {
      return NextResponse.json({ error: "Não autorizado" }, { status: 401 });
    }

    const { id } = params;

    // Verificar se a sessão existe e pertence ao usuário
    const sessaoEstudo = await prisma.sessaoEstudo.findUnique({
      where: { id },
    });

    if (!sessaoEstudo) {
      return NextResponse.json({ error: "Sessão não encontrada" }, { status: 404 });
    }

    if (sessaoEstudo.usuarioId !== session.id) {
      return NextResponse.json({ error: "Acesso negado" }, { status: 403 });
    }

    // Excluir a sessão
    await prisma.sessaoEstudo.delete({
      where: { id },
    });

    return NextResponse.json({ success: true });
  } catch (error) {
    return NextResponse.json(
      { error: "Erro ao excluir a sessão" },
      { status: 500 }
    );
  }
}
