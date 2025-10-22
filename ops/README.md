# Local Development Environment

## Quick Start

```bash
cd ops/

# Build and start all services
docker-compose up --build

# Start in background
docker-compose up -d

# View logs
docker-compose logs -f cpp-physics
docker-compose logs -f cpp-growth

# Stop everything
docker-compose down
```

## Services Running

- **mosquitto** (MQTT broker): `localhost:1883`
- **cpp-physics** (gRPC server): `localhost:50051`
- **cpp-growth** (MQTT publisher): publishes to `potato/growth` topic

## Testing

### Test MQTT broker
```bash
# Subscribe to growth updates
docker exec -it potato-mosquitto mosquitto_sub -t "potato/growth"

# Publish test message
docker exec -it potato-mosquitto mosquitto_pub -t "potato/growth" -m "test"
```

### Test gRPC service
```bash
# Install grpcurl first: brew install grpcurl

# List services
grpcurl -plaintext localhost:50051 list

# Call health check
grpcurl -plaintext localhost:50051 potato.PhysicsEngine.HealthCheck
```

## Network

All containers are on the `potato-net` bridge network (172.20.0.0/16).

Services can reach each other by name:
- `cpp-physics:50051` (gRPC)
- `mosquitto:1883` (MQTT)

## Rebuilding

```bash
# Rebuild specific service
docker-compose build cpp-physics

# Rebuild everything
docker-compose build

# Force rebuild and restart
docker-compose up --build --force-recreate
```
