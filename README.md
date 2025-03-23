# DewaterIQ Deployment Guide (Ubuntu + Docker + systemd)

This guide describes how to package your DewaterIQ project into a Docker image, export it as a `.tar` file, and deploy it on an Ubuntu server using systemd.

## Step 1: Build Docker Image Locally

Navigate to your project's root folder containing the Dockerfile, then run:

```
docker build -t dewateriq-qt6 .
```

Ensure the build completes successfully.

## Step 2: Export Docker Image to TAR File

Export the image you built to a `.tar` file:

```
docker save -o dewateriq-qt6.tar dewateriq-qt6
```

This creates a file named `dewateriq-qt6.tar` in your current directory.

## Step 3: Transfer TAR File to Server

Use scp or your preferred method to transfer the file to your Ubuntu server. Example:

```
scp dewateriq-qt6.tar ubuntu@yourserver:/home/ubuntu/
```

Replace `yourserver` with your server's IP address or hostname.

## Step 4: Load the Docker Image on the Server

SSH into your server, then run:

```
sudo docker load -i dewateriq-qt6.tar
```

Confirm the image loaded successfully:

```
sudo docker images
```

## Step 5: Create systemd Service on Server

Create the service file on your server:

```
sudo nano /etc/systemd/system/dewateriq.service
```

Paste this into the file:

```
[Unit]
Description=DewaterIQ Docker Service
After=docker.service
Requires=docker.service

[Service]
Restart=always
RestartSec=5

ExecStart=/usr/bin/docker run --rm --name dewateriq \
  -e QT_QPA_PLATFORM=offscreen \
  dewateriq-qt6

ExecStop=/usr/bin/docker stop dewateriq

[Install]
WantedBy=multi-user.target
```

Save the file and exit (Ctrl+O, Enter, Ctrl+X).

## Step 6: Start and Enable the Service

Run the following commands:

```
sudo systemctl daemon-reload
sudo systemctl enable dewateriq.service
sudo systemctl start dewateriq.service
```

## Step 7: Check Service Status and Logs

Check service status:

```
sudo systemctl status dewateriq.service
```

View real-time logs:

```
journalctl -u dewateriq.service -f
```

## Step 8: Stopping or Restarting the Service

To stop the service:

```
sudo systemctl stop dewateriq.service
```

To restart the service:

```
sudo systemctl restart dewateriq.service
```

## Additional Notes

- The `--rm` option in the service file removes the container upon stopping. Omit this if persistent container data or logs are needed.
- If you need to share folders or files, use Docker volumes (`-v`) within the `ExecStart` command.
