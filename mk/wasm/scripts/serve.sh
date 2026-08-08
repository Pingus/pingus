#!/usr/bin/env bash
# Serve a wasm build directory over HTTP and open a browser.
# Env: APP_NAME (default: index), optional SUPERTUX_WASM_PORT (default 8765)
# Run from the package directory or set PKG to that path.
set -euo pipefail

if [ -n "${PKG:-}" ]; then
  cd "$PKG"
fi

app_name="${APP_NAME:-pingus}"
port="${SUPERTUX_WASM_PORT:-8765}"

port_file=$(mktemp)
server_pid=
trap 'kill "$server_pid" 2>/dev/null || true; rm -f "$port_file"' EXIT

python3 -c '
import http.server, socketserver, sys
port_file, port = sys.argv[1], int(sys.argv[2])
class Quiet(http.server.SimpleHTTPRequestHandler):
    def log_message(self, *a): pass
socketserver.TCPServer.allow_reuse_address = True
try:
    httpd = socketserver.TCPServer(("127.0.0.1", port), Quiet)
except OSError as e:
    sys.stderr.write(
        "error: cannot bind 127.0.0.1:%s (%s)\n"
        "       set SUPERTUX_WASM_PORT to a free port\n" % (port, e))
    sys.exit(1)
open(port_file, "w").write(str(httpd.server_address[1]))
httpd.serve_forever()
' "$port_file" "$port" &
server_pid=$!

for i in $(seq 1 50); do
  [ -s "$port_file" ] && break
  sleep 0.05
done
if [ ! -s "$port_file" ]; then
  echo "error: local HTTP server failed to start on port $port" >&2
  exit 1
fi
port=$(cat "$port_file")
html="${app_name}.html"
if [ ! -f "$html" ]; then
  html=$(ls -1 *.html 2>/dev/null | head -1 || true)
fi
url="http://127.0.0.1:${port}/${html}"
echo "Serving ${app_name} at $url  (Ctrl-C to stop)"
echo "  IDBFS origin is tied to this host:port — keep the port stable to retain saves."

if [ -n "${BROWSER:-}" ]; then
  "$BROWSER" "$url" >/dev/null 2>&1 || true
elif command -v xdg-open >/dev/null 2>&1; then
  xdg-open "$url" >/dev/null 2>&1 || true
fi

wait "$server_pid"
