import json
import numpy as np
from pybullet_envs.deep_mimic.learning.ppo_agent import PPOAgent
from pybullet_envs.deep_mimic.learning.meta_ppo_agent import MetaPPOAgent
from pybullet_envs.deep_mimic.learning.meta_agent import METAAgent
import pybullet_data

AGENT_TYPE_KEY = "AgentType"


def build_agent(world, id, file):
  agent = None
  with open(pybullet_data.getDataPath() + "/" + file) as data_file:
    json_data = json.load(data_file)

    assert AGENT_TYPE_KEY in json_data
    agent_type = json_data[AGENT_TYPE_KEY]

    if (agent_type == PPOAgent.NAME):
      agent = PPOAgent(world, id, json_data)
    if (agent_type == MetaPPOAgent.NAME):
      agent = MetaPPOAgent(world, id, json_data)
    else:
      assert False, 'Unsupported agent type: ' + agent_type

  return agent

def build_meta_agent(world, id, file):
  agent = None
  with open(pybullet_data.getDataPath() + "/" + file) as data_file:
    json_data = json.load(data_file)

    assert AGENT_TYPE_KEY in json_data
    agent_type = json_data[AGENT_TYPE_KEY]

    if (agent_type == PPOAgent.NAME):
      agent = METAAgent(world, id, json_data)
    else:
      assert False, 'Unsupported agent type: ' + agent_type

  return agent