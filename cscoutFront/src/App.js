import React, { Component } from 'react';
import './App.css';
import Toolbar from './Components/Toolbar/Toolbar';
import Main from './Components/Main';
import Metrics from './Components/Metrics';
import SideDrawer from './Components/Toolbar/SideDrawer';
import Backdrop from './Components/Toolbar/Backdrop';
import FBrowse from './Components/Files/FileBrowser/FBrowse';
import SourceControl from './Components/Files/Source/SourceControl';
import Source from './Components/Files/Source/Source';
import FileSearch from './Components/Files/FileSearch/FileSearch';
import FilePage from './Components/Files/FilePage/FilePage';
import Identifier from './Components/Identifiers/Identifier';
import IdentifierSearch from './Components/Identifiers/IdentifierSearch';
import Fun from './Components/Functions/Fun';
import FunctionSearch from './Components/Functions/FunctionSearch';
import ReactSVG from 'react-svg';

class App extends Component {
  constructor() {
    super();
    this.state = {
      type: "homepage",
      sideDrawerOpen: false
    }
  }

  setType = (type) => {
    this.setState({
      sideDrawerOpen: false,
      type: type
    });
  };

  setTypeId = (type, id) => {
    this.setState({
      sideDrawerOpen: false,
      type: type,
      id: id
    })
  }

  setCall = (pos) => {
    this.setState({
      call: pos
    })
  }

  drawerToggleClickHandler = () => {
    this.setState((prevState) => {
      return { sideDrawerOpen: !prevState.sideDrawerOpen };
    });
  };

  backdropClickHandler = () => {
    this.setState({ sideDrawerOpen: false });
  };

  linkSrc = (obj) => {
    this.setState({
      type: "link",
      id: obj
    });
  }
  render() {
    let to_render;
    let backdrop;
    if (this.state.sideDrawerOpen) {
      backdrop = <Backdrop click={this.backdropClickHandler} />;
    }
    switch (this.state.type) {
      case ("filemetrics"):
        to_render = <Metrics type='file' />;
        break;
      case ("idmetrics"):
        to_render = <Metrics type='id' />;
        break;
      case ("funmetrics"):
        to_render = <Metrics type='fun' />;
        break;
      case ("homepage"):
        to_render = <Main changeType={this.setType} />;
        break;
      case ("browse"):
        to_render = <FBrowse type='top' changeType={this.setTypeId} />
        break;
      case ("qsource"):
        to_render = <Source id={this.state.id} f={this.state.call} type={'5'} changeType={this.setTypeId} />
        break;
      case ("filePage"):
        to_render = <FilePage id={this.state.id} changeType={this.setTypeId} />
        break;
      case ("searchf"):
        to_render = <div><FileSearch toFile={this.setTypeId} /></div>
        break;
      case ("id"):
        to_render = <div><Identifier id={this.state.id} changeType={this.setTypeId} /></div>
        break;
      case ("searchId"):
        to_render = <IdentifierSearch changeType={this.setTypeId} />
        break;
      case ("fun"):
        to_render = <Fun f={this.state.id} changeType={this.setTypeId} setCall={this.setCall}/>
        break;
      case("searchfun"):
        to_render = <FunctionSearch changeType={this.setTypeId} />
        break;
      case("graph"):
        to_render = <ReactSVG src={global.address + "cgraph.svg?all=1"}  
        // beforeInjection={svg => {
        //   svg.classList.add('svg-class-name')
        //   svg.setAttribute('style', 'width: 200px')
        // }}
        />
        break;
      default:
        to_render = <h1>{this.state.type}</h1>
    }
    return (
      <div className="App">
        <SideDrawer changeType={this.setType} show={this.state.sideDrawerOpen}
          drawerClickHandler={this.drawerToggleClickHandler} />
        {backdrop}
        <Toolbar changeType={this.setType} style={{ zIndex: '20' }}
          drawerClickHandler={this.drawerToggleClickHandler}   loading={() => <span>Loading</span>}
          />
        {to_render}
      </div>
    );
  }
}

export default App;
