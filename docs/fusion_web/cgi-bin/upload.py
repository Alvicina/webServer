#!/usr/bin/python3
"""
import cgi, os

form = cgi.FieldStorage()

# Get filename here
fileitem = form['filename']

# Test if the file was uploaded
if fileitem.filename:
   open(os.getcwd() + '/docs/fusion_web/cgi-bin/tmp/' + os.path.basename(fileitem.filename), 'wb').write(fileitem.file.read())
   message = 'The file "' + os.path.basename(fileitem.filename) + '" was uploaded to ' + os.getcwd() + '/docs/fusion_web/cgi-bin/tmp'
else:
   message = 'Uploading Failed'

#print("Content-Type: text/html;charset=utf-8")
#print ("Content-type:text/html\r\n")
print("<H1> " + message + " </H1>")
"""


#!/usr/bin/python3

import cgi
import os

# Obtener los datos del formulario
form = cgi.FieldStorage()

# Verificar si el campo 'filename' está presente en el formulario
fileitem = form['filename'] if 'filename' in form else None

# Mensaje de resultado
message = 'Uploading Failed'

# Verificar si el archivo fue subido
if fileitem and hasattr(fileitem, 'filename') and fileitem.filename:
    # Crear la ruta para guardar el archivo
    upload_dir = os.path.join(os.getcwd(), 'docs/fusion_web/cgi-bin/tmp/')
    os.makedirs(upload_dir, exist_ok=True)
    file_path = os.path.join(upload_dir, os.path.basename(fileitem.filename))
    
    # Guardar el archivo en la ruta especificada
    with open(file_path, 'wb') as f:
        f.write(fileitem.file.read())
    
    message = f'The file "{os.path.basename(fileitem.filename)}" was uploaded to {file_path}'

# Imprimir el encabezado de contenido y el mensaje
print("Content-Type: text/html;charset=utf-8")
print()  # Línea en blanco necesaria para separar encabezado y contenido
print(f"<h1>{message}</h1>")





