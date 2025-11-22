import socket
import threading
import time
import pytest
import paho.mqtt.client as mqtt

# ------------------------
# MQTT Configuration
# ------------------------
MQTT_BROKER = "test.mosquitto.org"
MQTT_PORT = 1883
MQTT_TOPIC = "proxelera/data"
MQTT_TIMEOUT = 10

# ------------------------
# TCP Server Configuration
# ------------------------
TCP_SERVER_IP = "127.0.0.1"
TCP_SERVER_PORT = 8080

# ------------------------
# MQTT Subscriber
# ------------------------
received_messages = []

def on_message(client, userdata, msg):
    """Callback for incoming MQTT messages"""
    print(f"MQTT Received: {msg.payload.decode()}")
    received_messages.append(msg.payload.decode())

def start_mqtt_listener():
    """Start MQTT subscriber in a separate thread"""
    client = mqtt.Client("TestSubscriber")
    client.on_message = on_message
    client.connect(MQTT_BROKER, MQTT_PORT)
    client.subscribe(MQTT_TOPIC)
    client.loop_start()
    return client

# ------------------------
# TCP Client Helper
# ------------------------
def tcp_send_message(message):
    """Send a message to the TCP server"""
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((TCP_SERVER_IP, TCP_SERVER_PORT))
        sock.sendall(message.encode())
        sock.close()
    except Exception as e:
        print(f"TCP send failed: {e}")

# ------------------------
# Test Cases
# ------------------------
def wait_for_message(msg, timeout=5):
    """Wait until the expected message appears in MQTT subscriber"""
    end_time = time.time() + timeout
    while msg not in received_messages and time.time() < end_time:
        time.sleep(0.1)
    return msg in received_messages

def test_single_client_message():
    """Test a single TCP client message"""
    received_messages.clear()
    mqtt_client = start_mqtt_listener()
    time.sleep(1)  # allow MQTT subscription to start

    test_msg = "Hello from test_single_client"
    tcp_send_message(test_msg)

    assert wait_for_message(test_msg), "Message not received via MQTT"

    mqtt_client.loop_stop()
    mqtt_client.disconnect()

def test_multiple_clients_messages():
    """Test multiple TCP clients sending messages concurrently"""
    received_messages.clear()
    mqtt_client = start_mqtt_listener()
    time.sleep(1)

    messages = [f"Message {i}" for i in range(10)]
    threads = []

    # Start multiple TCP clients
    for msg in messages:
        t = threading.Thread(target=tcp_send_message, args=(msg,))
        threads.append(t)
        t.start()

    for t in threads:
        t.join()

    # Wait for all messages to appear
    for msg in messages:
        assert wait_for_message(msg), f"{msg} not received via MQTT"

    mqtt_client.loop_stop()
    mqtt_client.disconnect()

def test_empty_message():
    """Test sending an empty message"""
    received_messages.clear()
    mqtt_client = start_mqtt_listener()
    time.sleep(1)

    tcp_send_message("")  # send empty message

    time.sleep(1)
    # Usually empty messages are ignored
    assert "" not in received_messages, "Empty message should not be published"

    mqtt_client.loop_stop()
    mqtt_client.disconnect()
