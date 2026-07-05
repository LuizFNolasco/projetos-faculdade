const { Client } = require('pg');

const client = new Client({
  connectionString: 'postgresql://workandrest:workandrest@localhost:5432/workandrest?schema=public'
});

async function main() {
  await client.connect();
  const res = await client.query('SELECT nome, email, senha FROM "usuarios"');
  console.log("Usuarios no banco:", res.rows);
  await client.end();
}

main().catch(console.error);
