import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart

# SMTP server configuration
smtp_server = 'smtp.gmail.com'
smtp_port = 587
sender_email = '209997@student.upm.edu.my'
sender_password = 'Zulhisyam4'
receiver_email = '212213@student.upm.edu.my'

# Create a SMTP session
s = smtplib.SMTP(smtp_server, smtp_port)
s.starttls()  # Start TLS encryption

# Login to Gmail
s.login(sender_email, sender_password)

# Email content
subject = 'ISEYYYYY [PAPIN]'
message = """
Dear recipient,

This is a test email message from Python.

Best regards,
Sender
"""

# Construct the message
msg = MIMEMultipart()
msg['From'] = sender_email
msg['To'] = receiver_email
msg['Subject'] = subject

# Attach the message body
msg.attach(MIMEText(message, 'plain'))

# Send the email
s.sendmail(sender_email, receiver_email, msg.as_string())

# Quit the session
s.quit()
