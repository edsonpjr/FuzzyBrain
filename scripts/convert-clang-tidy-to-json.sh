#!/bin/bash
# Script para converter clang-tidy output para GitLab Code Quality report format

# Se não houver build.log, criar um vazio
if [ ! -f "build.log" ]; then
	echo "No build.log found. Creating empty report."
	echo "[]" > clang-tidy-report.json
	exit 0
fi

# Converte para formato JSON esperado pelo GitLab
python3 << 'EOF'
import json
import re

issues = []

try:
	with open('build.log', 'r') as f:
		for line in f:
			# Padrão: arquivo:linha:coluna: warning/error: mensagem [check-name]
			# Exemplo: /path/file.cpp:10:5: warning: unused variable [some-check]
			match = re.match(r'(.+?):(\d+):(\d+): (warning|error): (.+)', line)
			if match:
				file_path, line_num, col, severity, message = match.groups()

				# Remove check name se estiver entre colchetes
				message = re.sub(r'\s*\[.*?\]\s*$', '', message)

				issues.append({
					"type": "issue",
					"check_name": "clang-tidy",
					"description": message.strip(),
					"categories": ["Style"],
					"severity": "major" if severity == "error" else "minor",
					"location": {
						"path": file_path.replace('/builds/', ''),
						"lines": {
							"begin": int(line_num)
						}
					}
				})

	with open('clang-tidy-report.json', 'w') as f:
		json.dump(issues, f, indent=2)

	print(f"✓ Generated report with {len(issues)} issues")

except Exception as e:
	print(f"Error: {e}")
	with open('clang-tidy-report.json', 'w') as f:
		json.dump([], f)
EOF
