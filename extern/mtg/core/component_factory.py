import logging
from typing import Callable
from dataclasses import dataclass
from core.component import Component, add_post_init
from core.models.test_helper import TestHelper

logger = logging.getLogger(__name__)

# The factory class for creating Components

_registry = {}

def register(wrapped_class: Component) -> Callable:
   name = wrapped_class.__name__
   # Sanity check to make sure we do not register a class that already exists in the Factory
   if name in _registry:
      logger.warning(f'Component {name} already exists. Will replace it')

   # Add the argument (which is a 'class') to the registry
   _registry[name] = add_post_init(wrapped_class)
   return wrapped_class


def mtg_dataclass(wrapped_class: Component) -> Callable:
   return add_post_init(dataclass(wrapped_class))

def get_component(name: str) -> 'Component':
   # Factory command to create a Component
   component_class = _registry[name]
   return component_class
