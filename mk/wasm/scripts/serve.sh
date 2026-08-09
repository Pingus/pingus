#!/usr/bin/env bash
# Serve a wasm build directory over HTTP and open a browser.
# Env: APP_NAME (default: pingus), optional PINGUS_WASM_PORT (default 8765; was SUPERTUX_WASM_PORT)
#      PINGUS_WASM_OPEN_QUERY — extra query string (e.g. verbose=1&debug=1)
# Run from the package directory or set PKG to that path.
set -euo pipefail

# CLI flags (from: nix run .#pingus-wasm -- --debug --verbose)
# Map to the same URL query keys shell.html already understands.
cli_query=()
while [ "$#" -gt 0 ]; do
  case "$1" in
    --debug)   cli_query+=("debug=1"); shift ;;
    --verbose|-v) cli_query+=("verbose=1"); shift ;;
    --help|-h)
      echo "Usage: serve.sh [--debug] [--verbose]"
      echo "  --debug    open with ?debug=1  (Pingus -D / developer-mode)"
      echo "  --verbose  open with ?verbose=1 (Pingus -v)"
      echo "Env: APP_NAME, PKG, PINGUS_WASM_PORT, PINGUS_WASM_OPEN_QUERY, BROWSER"
      exit 0
      ;;
    --) shift; break ;;
    -*)
      echo "error: unknown option: $1 (try --help)" >&2
      exit 1
      ;;
    *)
      echo "error: unexpected argument: $1 (try --help)" >&2
      exit 1
      ;;
  esac
done

if [ -n "${PKG:-}" ]; then
  cd "$PKG"
fi

app_name="${APP_NAME:-pingus}"
port="${PINGUS_WASM_PORT:-8765}"

port_file=$(mktemp)
server_pid=
trap 'kill "$server_pid" 2>/dev/null || true; rm -f "$port_file"' EXIT

# Always no-store so a rebuilt .js/.wasm/.data is not kept after a frozen-tab
# reload. Busy-wait builds can lock the renderer; users then hard-reload into
# a cached old binary and think the new one is still broken.
python3 -c '
import http.server, socketserver, sys
port_file, port = sys.argv[1], int(sys.argv[2])

class NoCacheHandler(http.server.SimpleHTTPRequestHandler):
    def end_headers(self):
        self.send_header("Cache-Control", "no-store, no-cache, must-revalidate, max-age=0")
        self.send_header("Pragma", "no-cache")
        self.send_header("Expires", "0")
        super().end_headers()
    def log_message(self, *a):
        pass

socketserver.TCPServer.allow_reuse_address = True
try:
    httpd = socketserver.TCPServer(("127.0.0.1", port), NoCacheHandler)
except OSError as e:
    sys.stderr.write(
        "error: cannot bind 127.0.0.1:%s (%s)\n"
        "       set PINGUS_WASM_PORT to a free port\n" % (port, e))
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

# Cache-bust query so the HTML document itself is a new URL even if a proxy
# ignored Cache-Control. Optional PINGUS_WASM_OPEN_QUERY and --debug/--verbose
# append flags (shell.html maps ?debug=1 → -D, ?verbose=1 → -v).
bust=$(date +%s)
parts=("v=${bust}")
extra="${PINGUS_WASM_OPEN_QUERY:-}"
if [ -n "$extra" ]; then
  parts+=("$extra")
fi
if [ "${#cli_query[@]}" -gt 0 ]; then
  parts+=("${cli_query[@]}")
fi
q=$(IFS='&'; echo "${parts[*]}")
url="http://127.0.0.1:${port}/${html}?${q}"

echo "Serving ${app_name} at $url  (Ctrl-C to stop)"
echo "  Cache-Control: no-store on all responses; ?v=… busts document cache."
echo "  IDBFS origin is tied to this host:port — keep the port stable to retain saves."
echo "  If a tab is frozen (busy-wait CPU spin): kill that tab/process, then open the URL above."

if [ -n "${BROWSER:-}" ]; then
  "$BROWSER" "$url" >/dev/null 2>&1 || true
elif command -v xdg-open >/dev/null 2>&1; then
  xdg-open "$url" >/dev/null 2>&1 || true
fi

wait "$server_pid"
