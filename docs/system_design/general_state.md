# General state

You can think how phone link firmware works internally from this flow chart:

```mermaid
---
title: General state
---
stateDiagram-v2
    [*] --> Standby

    Standby --> Ringing
    Ringing --> Standby
    Ringing --> CallActive
    CallActive --> Standby
    Standby --> Off-Hook
    OffHook --> Standby
    OffHook --> CallActive
    Standby --> [*]
```